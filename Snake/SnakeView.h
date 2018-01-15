
// SnakeView.h : interface of the CSnakeView class
//

#pragma once
#include "CSnake.h"


class CSnakeView : public CView
{
protected: // create from serialization only
	CSnakeView();
	DECLARE_DYNCREATE(CSnakeView)

// Attributes
public:
	Snake snake; // Snake object

	CRect window_rect;
	CRect game_rect;
	CRect score_rect;
	CRect highest_rect;
	CRect food_rect;
	CRect snake_rect;
	CRect info_rect;
	// All the rectangles for drawing

	CString score;
	CString highest;
	CString info;
	// String for two scores

	UINT m_score;
	UINT m_highest;
	// Integer for two scores

	CBitmap m_bg;
	CBitmap m_bg_light;
	CBitmap m_bg_dark;
	// Three different background

	CImage m_food;
	CImage m_trophy;
	// Two image for display

	CRect m_food_rect;
	CRect m_trophy_rect;
	// Two rectangles for above two images

	CFont m_font; // new font for drawing 
	BOOL m_pause; // if the game is paused
	BOOL m_died; // if the snake the died

// Operations
public:
	CSnakeDoc* GetDocument() const;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSnakeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnLeft();
	afx_msg void OnRight();
	afx_msg void OnSTART();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnGamePause();
	afx_msg void OnGameContinue();
	afx_msg void OnGameStop();
};

#ifndef _DEBUG  // debug version in SnakeView.cpp
inline CSnakeDoc* CSnakeView::GetDocument() const
   { return reinterpret_cast<CSnakeDoc*>(m_pDocument); }
#endif

