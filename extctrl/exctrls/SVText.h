#pragma once

SNSBEGIN

class SVText: public SWindow
{
	DEF_SOBJECT(SWindow,L"vtext")
public:
	SVText(void);
	~SVText(void);

protected:
	SIZE MeasureContent(int wid, int hei) override;
	void OnPaint(IRenderTarget *pRT);

	SOUI_MSG_MAP_BEGIN()
		MSG_WM_PAINT_EX(OnPaint)
	SOUI_MSG_MAP_END()
};

SNSEND