#include "stdafx.h"
#include "SScrollSubtitles.h"

SNSBEGIN

SScrollSubtitles::SScrollSubtitles():m_nSpeed(1), m_nLineHeight(30), m_xInterval(10), m_iNextLine(0), m_yOffset(10),m_nCount(0)
	{
	}


	SScrollSubtitles::~SScrollSubtitles()
	{
		SPOSITION pos = m_lstSubtitles.GetHeadPosition();
		while (pos)
		{
			SubtitlesInfo *pInfo = m_lstSubtitles.GetNext(pos);
			delete pInfo;
		}
		m_lstSubtitles.RemoveAll();
	}

	BOOL SScrollSubtitles::AddSubtitles(LPCSTR pszSubtitle, int nType)
	{
		if (!m_mapTypeFont.Lookup(nType))
		{
			return false;
		}
		SStringT strSubtitle = S_CA2T(pszSubtitle, CP_UTF8);
		IRenderTarget *pRT = NULL;
		GETRENDERFACTORY->CreateRenderTarget(&pRT, 0, 0);
		BeforePaintEx(pRT);
		CSize size;
		IRenderObj *pOldObj = NULL;
		pRT->SelectObject(m_mapTypeFont[nType].font, &pOldObj);
		pRT->MeasureText(strSubtitle, strSubtitle.GetLength(), &size);
		pRT->SelectObject(pOldObj, NULL);
		pRT->Release();

		if(m_iNextLine!=-1){
			SubtitlesInfo *pInfo = new SubtitlesInfo;
			pInfo->strText = strSubtitle;
			pInfo->nType = nType;
			pInfo->sz = size;
			pInfo->iRow = m_iNextLine;
			pInfo->xOffset = m_arrLineTail[m_iNextLine] + m_xInterval;

			CRect rcClient = GetClientRect();
			if (pInfo->xOffset < rcClient.Width())
				pInfo->xOffset = rcClient.Width();

			m_lstSubtitles.AddTail(pInfo);

			m_arrLineTail[m_iNextLine] = pInfo->xOffset + pInfo->sz.cx +  m_xInterval;
		}

		m_iNextLine = -1;
		int nMinX = 100000;
		for (UINT i = 0; i < m_arrLineTail.GetCount(); i++)
		{
			if (m_arrLineTail[i] < nMinX)
			{
				nMinX = m_arrLineTail[i];
				m_iNextLine = i;
			}
		}
		return true;
	}

	void SScrollSubtitles::OnShowWindow(BOOL bShow, UINT nStatus)
	{
		__super::OnShowWindow(bShow, nStatus);

		if (IsVisible(TRUE))
			GetContainer()->RegisterTimelineHandler(this);
		else
			GetContainer()->UnregisterTimelineHandler(this);
	}

	void SScrollSubtitles::OnPaint(IRenderTarget * pRT)
	{
		SPainter painter;
		BeforePaint(pRT, painter);

		CRect rcClient = GetClientRect();
		int nWid = rcClient.Width();

		SPOSITION pos = m_lstSubtitles.GetHeadPosition();
		while (pos)
		{
			SubtitlesInfo *pInfo = m_lstSubtitles.GetNext(pos);
			if (pInfo->xOffset < nWid)
			{
				int yPos = pInfo->iRow * m_nLineHeight + m_yOffset + (m_nLineHeight - pInfo->sz.cy)/2;
				int xPos = pInfo->xOffset;
				SASSERT(m_mapTypeFont.Lookup(pInfo->nType));
				TypeFont tf = m_mapTypeFont[pInfo->nType];
				IRenderObj *pOldFont = NULL;
				pRT->SelectObject(tf.font, &pOldFont);
				COLORREF crOld = pRT->SetTextColor(tf.color);
				pRT->TextOut(rcClient.left + xPos, rcClient.top + yPos, pInfo->strText, pInfo->strText.GetLength());
				pRT->SetTextColor(crOld);
				pRT->SelectObject(pOldFont, NULL);
			}
			if (pInfo->xOffset > nWid) break;
		}
		AfterPaint(pRT, painter);
	}


	void SScrollSubtitles::OnNextFrame()
	{
		if(!IsVisible(TRUE))
			return;
		if((m_nCount++)%5!=0)
		{
			return;
		}
		m_nCount = 1;
		CRect rcClient = GetClientRect();
		SPOSITION pos = m_lstSubtitles.GetHeadPosition();
		while (pos)
		{
			SPOSITION posPrev = pos;
			SubtitlesInfo *pData = m_lstSubtitles.GetNext(pos);

			CPoint pt;
			pt.y = pData->iRow * m_nLineHeight + m_yOffset + (m_nLineHeight - pData->sz.cy) / 2;
			pt.x = pData->xOffset;
			pt.Offset(rcClient.TopLeft());
			CRect rc(pt, pData->sz);
			rc.left -= m_nSpeed; //increase invalid rect
			rc.right += m_nSpeed;
			InvalidateRect(rc);

			pData->xOffset -= m_nSpeed;

			if (pData->xOffset + pData->sz.cx <= 0)
			{
				m_lstSubtitles.RemoveAt(posPrev);
				delete pData;
			}
		}
		for (UINT i = 0; i < m_arrLineTail.GetCount(); i++)
		{
			m_arrLineTail[i] -= m_nSpeed;
			if (m_arrLineTail[i] < 0)
			{
				m_arrLineTail[i] = 0;
			}
		}
	}

	void SScrollSubtitles::OnInitFinished(IXmlNode * pNode)
	{
		SXmlNode xmlNode(pNode);
		SXmlNode xmlFonts = xmlNode.child(L"fonts");
		if (xmlFonts)
		{
			SXmlNode xmlTypeFont = xmlFonts.child(L"typeFont");
			while (xmlTypeFont)
			{
				int nType = xmlTypeFont.attribute(L"type").as_int();
				if (!m_mapTypeFont.Lookup(nType))
				{
					TypeFont tf;
					tf.font = SFontPool::GetFont(xmlTypeFont.attribute(L"font").as_string(), GetScale());
					tf.color = GETCOLOR(xmlTypeFont.attribute(L"color").as_string());
					m_mapTypeFont[nType] = tf;
				}
				xmlTypeFont = xmlTypeFont.next_sibling(L"typeFont");
			}
		}
	}
	
	LRESULT SScrollSubtitles::OnAttrRows(const SStringW & strValue, BOOL bLoading)
	{
		if (!bLoading) return E_ABORT;
		int nRows = _wtoi(strValue);
		if (nRows < 2 || nRows>20) return E_ABORT;
		m_arrLineTail.SetCount(nRows);
		for (int i = 0; i < nRows; i++)
		{
			m_arrLineTail[i] = 0;
		}
		return S_OK;
	}
SNSEND