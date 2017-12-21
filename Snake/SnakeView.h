
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
	Snake snake;
	CRect window_rect;
	CRect game_rect;
	CRect score_rect;
	CRect food_rect;
	CRect snake_rect;
	CBrush bg_brush; // background color
	CBrush score_brush; // score region color
	CBrush food_brush; // food color
	CBrush snake_brush; // snake color
	CString score;
	//CImage m_bg;
	CBitmap m_bg;

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
};

#ifndef _DEBUG  // debug version in SnakeView.cpp
inline CSnakeDoc* CSnakeView::GetDocument() const
   { return reinterpret_cast<CSnakeDoc*>(m_pDocument); }
#endif

