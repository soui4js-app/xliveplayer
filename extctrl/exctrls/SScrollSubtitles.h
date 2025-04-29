#pragma once
#include <interface/SCtrl-i.h>

SNSBEGIN

DEFINE_SGUID(IID_IScrollSubtitles, 0xB12EEA51, 0xA9E6, 0x45cf, 0x8f, 0x7a, 0x9D, 0xad, 0xa3, 0xa5, 0x90, 0x9f);
#undef INTERFACE
#define INTERFACE IScrollSubtitles
DECLARE_INTERFACE_(IScrollSubtitles, ICtrl)
{
	DECLARE_CLASS_SIID(IID_IScrollSubtitles)

	/**
 * @brief 增加引用计数
 * @return 新引用计数
 */
	STDMETHOD_(long, AddRef)(THIS) PURE;

	/**
	 * @brief 减少引用计数
	 * @return 新引用计数
	 */
	STDMETHOD_(long, Release)(THIS) PURE;

	/**
	 * @brief 释放对象
	 * @return void
	 */
	STDMETHOD_(void, OnFinalRelease)(THIS) PURE;

	//////////////////////////////////////////////////////////////////////////

	/**
	 * @brief 转换为IWindow*接口
	 * @return IWindow*接口
	 */
	STDMETHOD_(IWindow*, ToIWindow)(THIS) PURE;

	//////////////////////////////////////////////////////////////////////////
	STDMETHOD_(BOOL, AddSubtitles)(THIS_ LPCSTR pszLine, int nType DEF_VAL(0)) PURE;
};

class SScrollSubtitles : public TWindowProxy<IScrollSubtitles>, public ITimelineHandler
	{
		DEF_SOBJECT(SWindow,L"scrollSubtitles")
	public:
		SScrollSubtitles();
		~SScrollSubtitles();

		STDMETHOD_(BOOL, AddSubtitles)(THIS_ LPCSTR pszLine, int nType DEF_VAL(0)) OVERRIDE;
	protected:// 通过 ITimelineHandler 继承
		void WINAPI OnNextFrame() override;

	protected:
		void WINAPI OnInitFinished(IXmlNode * xmlNode) override;

	protected:
		LRESULT OnAttrRows(const SStringW & strValue, BOOL bLoading);
		SOUI_ATTRS_BEGIN()
			ATTR_INT(L"speed", m_nSpeed, FALSE)
			ATTR_CUSTOM(L"rows", OnAttrRows)
			ATTR_INT(L"xInterval", m_xInterval, TRUE)
			ATTR_INT(L"lineHeight", m_nLineHeight,TRUE)
			ATTR_INT(L"yOffset",m_yOffset,TRUE)
		SOUI_ATTRS_END()

	protected:
		void OnShowWindow(BOOL bShow, UINT nStatus);
		void OnPaint(IRenderTarget *pRT);

		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
			MSG_WM_SHOWWINDOW(OnShowWindow)
		SOUI_MSG_MAP_END()

	private:
		struct SubtitlesInfo
		{
			int  iRow;		//显示行
			long xOffset;   //显示偏移
			SIZE sz;		//占用空间
			SStringT strText; //显示内容
			int nType;		  //type
		};

		struct TypeFont {
			SAutoRefPtr<IFont> font;
			COLORREF		   color;
		};

		SList<SubtitlesInfo*> m_lstSubtitles;
		int					  m_nLineHeight;//行间距
		int					  m_xInterval;//水平间距
		int					  m_yOffset;
		int					  m_nSpeed;//每次步进的像素

		SArray<int>			  m_arrLineTail;//tail offset for lines.
		int					  m_iNextLine;  //next insert line

		SMap<int,TypeFont>      m_mapTypeFont;
		int					  m_nCount;
	};

SNSEND