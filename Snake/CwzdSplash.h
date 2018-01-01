#pragma once

// Class for splash that is displayed before the application
class CwzdSplash : public CWnd
{
	DECLARE_DYNAMIC(CwzdSplash)

protected:
	DECLARE_MESSAGE_MAP()

public:
	CBitmap m_bitmap; // the bitmap for displaying

public:
	CwzdSplash();
	virtual ~CwzdSplash();
	void Create(UINT nBitmapID);

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

