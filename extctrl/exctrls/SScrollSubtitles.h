#pragma once
#include <interface/SCtrl-i.h>

SNSBEGIN

#undef INTERFACE
#define INTERFACE IScrollSubtitles
DECLARE_INTERFACE_IID_(IScrollSubtitles, ICtrl,"B12EEA51-A9E6-45cf-8FA7-9DADA3A5909F") {
	/**
 * @brief �������ü���
 * @return �����ü���
 */
	STDMETHOD_(long, AddRef)(THIS) PURE;

	/**
	 * @brief �������ü���
	 * @return �����ü���
	 */
	STDMETHOD_(long, Release)(THIS) PURE;

	/**
	 * @brief �ͷŶ���
	 * @return void
	 */
	STDMETHOD_(void, OnFinalRelease)(THIS) PURE;

	//////////////////////////////////////////////////////////////////////////

	/**
	 * @brief ת��ΪIWindow*�ӿ�
	 * @return IWindow*�ӿ�
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
	protected:// ͨ�� ITimelineHandler �̳�
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
			int  iRow;		//��ʾ��
			long xOffset;   //��ʾƫ��
			SIZE sz;		//ռ�ÿռ�
			SStringT strText; //��ʾ����
			int nType;		  //type
		};

		struct TypeFont {
			SAutoRefPtr<IFont> font;
			COLORREF		   color;
		};

		SList<SubtitlesInfo*> m_lstSubtitles;
		int					  m_nLineHeight;//�м��
		int					  m_xInterval;//ˮƽ���
		int					  m_yOffset;
		int					  m_nSpeed;//ÿ�β���������

		SArray<int>			  m_arrLineTail;//tail offset for lines.
		int					  m_iNextLine;  //next insert line

		SMap<int,TypeFont>      m_mapTypeFont;
		int					  m_nCount;
	};

SNSEND