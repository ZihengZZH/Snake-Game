#include "stdafx.h"
#include "CwzdSplash.h"

IMPLEMENT_DYNAMIC(CwzdSplash, CWnd)

CwzdSplash::CwzdSplash()
{
}


CwzdSplash::~CwzdSplash()
{
}


BEGIN_MESSAGE_MAP(CwzdSplash, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CwzdSplash::Create(UINT nBitmapID)
{
	m_bitmap.LoadBitmap(nBitmapID);
	BITMAP bitmap;
	m_bitmap.GetBitmap(&bitmap);

	CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, 
		bitmap.bmWidth, bitmap.bmHeight, NULL, NULL);
}


void CwzdSplash::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	BITMAP bitmap;
	m_bitmap.GetBitmap(&bitmap);
	CDC dcComp;
	dcComp.CreateCompatibleDC(&dc);
	dcComp.SelectObject(&m_bitmap);
	// draw bitmap
	dc.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, &dcComp, 0, 0, SRCCOPY);
}


void CwzdSplash::OnTimer(UINT_PTR nIDEvent)
{
	DestroyWindow();
}
