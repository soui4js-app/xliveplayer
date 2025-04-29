#include "stdafx.h"
#include "SdlPresenter.h"
#include <helper/SFunctor.hpp>
#include <Mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#define  kLogTag "SdlPresenter"

SNSBEGIN

//////////////////////////////////////////////////////////////////////////
SdlPresenter::SdlPresenter(IHostWnd *pHostWnd)
:m_pHostWnd(pHostWnd)
,m_sdlWnd(NULL)
,m_sdlRenderer(NULL)
,m_wndTexture(NULL)
,m_bDirty(FALSE)
{
	m_commgr.CreateTaskLoop((IObjRef**)&m_renderLoop);
	m_renderLoop->start("sdl_presenter_thread", Normal);
	auto task = new SFunctor0<SdlPresenter,void (SdlPresenter::*)()>(this,&SdlPresenter::_OnHeartBeat);
	m_renderLoop->setHeartBeatTask(task,10);
	task->Release();
}

SdlPresenter::~SdlPresenter(void)
{
	m_renderLoop->stop();
	m_renderLoop = NULL;
}


void SdlPresenter::_OnHeartBeat()
{
	UpdateRender();
}

void SdlPresenter::_OnHostCreate()
{
	m_sdlRenderer = SDL_CreateRenderer(m_sdlWnd, -1, 0);
}

void SdlPresenter::OnHostCreate(THIS)
{
	m_sdlWnd = SDL_CreateWindowFrom(m_pHostWnd->GetHwnd());
	if(!m_sdlWnd)
		return ;
	STaskHelper::post(m_renderLoop, this, &SdlPresenter::_OnHostCreate, true);//sync call
}

void SdlPresenter::_OnHostDestroy()
{
	TextureMap::iterator it = m_videoTextures.begin();
	while (it != m_videoTextures.end())
	{
		SDL_DestroyTexture(it->second.texture);
		it++;
	}
	m_videoTextures.clear();

	if (m_wndTexture)
	{
		SDL_DestroyTexture(m_wndTexture);
		m_wndTexture = NULL;
	}

	if (m_sdlRenderer)
	{
		SDL_DestroyRenderer(m_sdlRenderer);
		m_sdlRenderer = NULL;
	}
}

void SdlPresenter::OnHostDestroy(THIS)
{
	STaskHelper::post(m_renderLoop, this, &SdlPresenter::_OnHostDestroy, true);//sync call
	if(m_sdlWnd)
	{
		SDL_DestroyWindow(m_sdlWnd);
		m_sdlWnd = NULL;
	}
}

void SdlPresenter::OnHostResize(THIS_ SIZE szHost)
{
}


void SdlPresenter::_OnHostPresent(HostTextureInfo info)
{
	if (!m_wndTexture || m_szWndTexture.cx != info.nWid || m_szWndTexture.cy != info.nHei) {
		if(m_wndTexture)
			SDL_DestroyTexture(m_wndTexture);
		//create texture for window cache
		m_wndTexture = SDL_CreateTexture(m_sdlRenderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, info.nWid, info.nHei);
		m_szWndTexture.cx = info.nWid, m_szWndTexture.cy = info.nHei;
		SDL_SetTextureBlendMode(m_wndTexture, SDL_BLENDMODE_BLEND);
	}
	if (m_wndTexture)
	{		
		SDL_Rect rcSdl = { 0,0,info.nWid,info.nHei };
		void* pDst = NULL;
		int pitch = 0;
		SDL_LockTexture(m_wndTexture, &rcSdl, &pDst, &pitch);
		if (pDst && pitch == info.nWid * 4) {
			memcpy(pDst, info.buf.ptr(), pitch * rcSdl.h);
		}
		else {
			SLOGW() << "_OnHostPresent error";
		}
		SDL_UnlockTexture(m_wndTexture);
		m_bDirty = TRUE;
	}
}

void SdlPresenter::OnHostPresent(THIS_ HDC hdc,IRenderTarget *pMemRT,LPCRECT pRect,BYTE byAlpha)
{
	IBitmapS* pBmp = (IBitmapS*)pMemRT->GetCurrentObject(OT_BITMAP);
	HostTextureInfo info;
	info.nWid = pBmp->Width();
	info.nHei = pBmp->Height();
	info.buf = SSharedPtr<BYTE>(new BYTE[info.nWid * 4 * info.nHei]);
	LPBYTE pBit = (LPBYTE)pBmp->GetPixelBits();
	memcpy(info.buf.ptr(), pBit, info.nWid * 4 * info.nHei);
	STaskHelper::post(m_renderLoop, this, &SdlPresenter::_OnHostPresent, info, false);
}

void SdlPresenter::_SetCanvasPos(int canvasId, CRect rcCanvas,BOOL bVisible) {
	TextureMap::iterator it = m_videoTextures.find(canvasId);
	if (it == m_videoTextures.end())
	{
		VideoTexture vt = { NULL,0,0,0,rcCanvas,bVisible };
		std::pair<TextureMap::iterator, bool> res = m_videoTextures.insert(std::make_pair(canvasId, vt));
	}
	else {
		it->second.rcVideo = rcCanvas;
	}
	m_bDirty = TRUE;
}

void SdlPresenter::_SetCanvasVisible(int canvasId, BOOL bVisible)
{
	TextureMap::iterator it = m_videoTextures.find(canvasId);
	if (it != m_videoTextures.end())
	{
		it->second.bVisible = bVisible;
	}
	m_bDirty = TRUE;
}

BOOL SdlPresenter::OnCanvasPos(IEvtArgs* e)
{
	EventSwndPos* e2 = sobj_cast<EventSwndPos>(e);
	SWindow* pCanvas = sobj_cast<SWindow>(e->Sender());
	BOOL bVisible = pCanvas->IsVisible(TRUE);
	STaskHelper::post(m_renderLoop, this, &SdlPresenter::_SetCanvasPos, e2->Sender()->GetID(), e2->rcWnd, bVisible, false);
	return TRUE;
}

BOOL SdlPresenter::OnCanvasVisibleChange(IEvtArgs* e)
{
	EventSwndVisibleChanged* e2 = sobj_cast<EventSwndVisibleChanged>(e);
	STaskHelper::post(m_renderLoop, this, &SdlPresenter::_SetCanvasVisible, e2->Sender()->GetID(), e2->bVisible, false);
	return TRUE;
}

void SdlPresenter::AddVideo(int canvasId)
{
	IWindow* pCanvas = m_pHostWnd->FindIChildByID(canvasId);
	if (pCanvas) {
		CRect rcVideo;
		pCanvas->GetWindowRect(&rcVideo);
		BOOL bVisible = pCanvas->IsVisible(TRUE);
		STaskHelper::post(m_renderLoop, this, &SdlPresenter::_SetCanvasPos, canvasId, rcVideo, bVisible, false);
		pCanvas->SubscribeEvent(EventSwndPos::EventID, &Subscriber(&SdlPresenter::OnCanvasPos, this));
	}
}

void SdlPresenter::_RemoveVideo(int canvasId)
{
	TextureMap::iterator it = m_videoTextures.find(canvasId);
	if (it != m_videoTextures.end()) {
		if (it->second.texture) {
			SDL_DestroyTexture(it->second.texture);
		}
		m_videoTextures.erase(it);
		m_bDirty = TRUE;
	}
}

void SdlPresenter::RemoveVideo(int canvasId)
{
	STaskHelper::post(m_renderLoop, this, &SdlPresenter::_RemoveVideo, canvasId, false);
}

void SdlPresenter::_UpdateVideoCanvas(int canvasId, SComPtr<IAVframe> frame)
{
	TextureMap::iterator it = m_videoTextures.find(canvasId);
	if (it == m_videoTextures.end()) {
		return;
	}
	else {
		it->second.frame = frame;
	}
	if (it->second.texture && (frame->width() != it->second.txtWid || frame->height() != it->second.txtHei)) {
		SDL_DestroyTexture(it->second.texture);
		it->second.texture = NULL;
	}
	if (!it->second.texture) {
		it->second.texture = SDL_CreateTexture(m_sdlRenderer, SDL_PIXELFORMAT_IYUV,
			SDL_TEXTUREACCESS_STREAMING, it->second.frame->width(), it->second.frame->height());
		it->second.txtWid = frame->width();
		it->second.txtHei = frame->height();
	}
	if (!it->second.texture) {
		SLOGE() << "create texture error";
		return ;
	}
	int line1Offset = frame->lineSize()[0] * frame->height();
	int line2Offset = line1Offset + frame->lineSize()[1] * frame->height() / 2;

	SDL_UpdateYUVTexture(it->second.texture, NULL, frame->getData(),
		frame->lineSize()[0], frame->getData() + line1Offset,
		frame->lineSize()[1], frame->getData() + line2Offset,
		frame->lineSize()[2]);

	m_bDirty = TRUE;
}

//run on vodplayer thread
void SdlPresenter::UpdateVideoCanvas(int canvasId,IAVframe *frame)
{
	SComPtr<IAVframe> tmp(frame);
	STaskHelper::post(m_renderLoop, this, &SdlPresenter::_UpdateVideoCanvas, canvasId, tmp, false);
}

void SdlPresenter::UpdateRender()
{
	if(!m_bDirty)
		return;
	if(m_pHostWnd->IsIconic())
	{
		m_bDirty = FALSE;
		return;
	}
	SDL_RenderClear(m_sdlRenderer);
	//render video
	TextureMap::iterator it = m_videoTextures.begin();
	while(it != m_videoTextures.end()){
		if(it->second.bVisible && it->second.texture){
			CRect rcCanvas = it->second.rcVideo;
			float ratioSrc = it->second.frame->width()*1.0f/it->second.frame->height();
			float ratioDst = rcCanvas.Width()*1.0f/rcCanvas.Height();
			if(ratioSrc>ratioDst){
				int hei = rcCanvas.Width()/ratioSrc;
				int nOffset = (rcCanvas.Height()-hei)/2;
				rcCanvas.DeflateRect(0,nOffset);
				SDL_Rect rcDst = {rcCanvas.left,rcCanvas.top,rcCanvas.Width(),rcCanvas.Height()};
				SDL_RenderCopy(m_sdlRenderer, it->second.texture, NULL, &rcDst);
			}else
			{
				int wid = rcCanvas.Height()*ratioSrc;
				int nOffset = (rcCanvas.Width()-wid)/2;
				rcCanvas.DeflateRect(nOffset,0);
				SDL_Rect rcDst = {rcCanvas.left,rcCanvas.top,rcCanvas.Width(),rcCanvas.Height()};

				SDL_RenderCopy(m_sdlRenderer, it->second.texture, NULL, &rcDst);
			}
		}
		it++;
	}
	SASSERT(m_wndTexture);
	//render ui elements.
	SDL_RenderCopy(m_sdlRenderer, m_wndTexture, NULL, NULL);
	SDL_RenderPresent(m_sdlRenderer);
	m_bDirty = FALSE;
}



SNSEND