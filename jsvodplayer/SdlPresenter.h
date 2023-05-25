#pragma once
#include <interface/SHostPresenter-i.h>
#include <interface/staskloop-i.h>
#include <helper/obj-ref-impl.hpp>
#include <helper/SSharedPtr.hpp>
#include <commgr2.h>
#include <SDL.h>
#include <map>
#include <transvod-i.h>
using namespace transvod_win;
SNSBEGIN

class SdlPresenter : public TObjRefImpl<IHostPresenter>
{
	IHostWnd * m_pHostWnd;
	SAutoRefPtr<ITaskLoop> m_renderLoop;

	SDL_Window * m_sdlWnd;
	SDL_Renderer *m_sdlRenderer;

	SDL_Texture * m_wndTexture;
	CSize		  m_szWndTexture;
	struct VideoTexture{
		SComPtr<IAVframe> frame;
		SDL_Texture * texture;
		int txtWid,txtHei;
		CRect rcVideo;
		BOOL  bVisible;
	};
	typedef std::map<int,VideoTexture> TextureMap;
	TextureMap m_videoTextures;
	BOOL	m_bDirty;

	SComMgr2 m_commgr;

	struct HostTextureInfo {
		int nWid;
		int nHei;
		SSharedPtr<BYTE> buf;
	};

public:
	SdlPresenter(IHostWnd *pHostWnd);
	~SdlPresenter(void);

	void AddVideo(int canvasId);
	void RemoveVideo(int canvasId);

	void UpdateVideoCanvas(int canvasId,IAVframe *pFrame);
	void UpdateRender();

	IHostWnd* GetHostWnd() { return m_pHostWnd; }
protected:
	BOOL OnCanvasPos(IEvtArgs* e);
	BOOL OnCanvasVisibleChange(IEvtArgs* e);
protected:
	void _SetCanvasPos(int canvasId, CRect rcCanvas, BOOL bVisible);
	void _SetCanvasVisible(int canvasId, BOOL bVisible);
	void _RemoveVideo(int canvasId);
	void _OnHeartBeat();
	void _OnHostCreate();
	void _OnHostDestroy();
	void _OnHostPresent(HostTextureInfo info);
	void _UpdateVideoCanvas(int canvasId, SComPtr<IAVframe> frame);
public:
	STDMETHOD_(void,OnHostCreate)(THIS) OVERRIDE;
	STDMETHOD_(void,OnHostDestroy)(THIS) OVERRIDE;
	STDMETHOD_(void,OnHostResize)(THIS_ SIZE szHost) OVERRIDE;
	STDMETHOD_(void,OnHostPresent)(THIS_ HDC hdc,IRenderTarget *pMemRT,LPCRECT rcDirty,BYTE byAlpha) OVERRIDE;
	STDMETHOD_(void,OnHostAlpha)(THIS_ BYTE byAlpha) OVERRIDE;
};

SNSEND
