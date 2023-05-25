#include "StdAfx.h"
#include "SVText.h"

SNSBEGIN

SVText::SVText(void)
{
	m_bMsgTransparent = true;
}

SVText::~SVText(void)
{
}

SIZE SVText::MeasureContent(int wid, int hei)
{
	IRenderTarget* pRT=NULL;
	GETRENDERFACTORY->CreateRenderTarget(&pRT);
	BeforePaintEx(pRT);
	SStringT strText = GetWindowText();
	SIZE szRet;
	pRT->MeasureText(strText, strText.GetLength(), &szRet);
	pRT->Release();
	//swap cx and cy.
	int t = szRet.cx;
	szRet.cx = szRet.cy;
	szRet.cy = t;
	return szRet;
}

void SVText::OnPaint(IRenderTarget *pRT)
{
	SPainter painter;
	BeforePaint(pRT,painter);

	CRect rcClient=GetClientRect();
	SStringT strText = GetWindowText();
	
	CPoint pt = rcClient.CenterPoint();
	SIZE szTxt;
	pRT->MeasureText(strText, strText.GetLength(), &szTxt);
	pt.x += szTxt.cy / 2;
	pt.y = rcClient.top;
	pRT->TextOut(pt.x,pt.y,strText,strText.GetLength());
	AfterPaint(pRT,painter);
}

SNSEND