
// SnakeView.cpp : implementation of the CSnakeView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Snake.h"
#endif

#include "SnakeDoc.h"
#include "SnakeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSnakeView

IMPLEMENT_DYNCREATE(CSnakeView, CView)

BEGIN_MESSAGE_MAP(CSnakeView, CView)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_START, &CSnakeView::OnSTART)
END_MESSAGE_MAP()

// CSnakeView construction/destruction

CSnakeView::CSnakeView()
{
}

CSnakeView::~CSnakeView()
{
}

BOOL CSnakeView::PreCreateWindow(CREATESTRUCT& cs)
{
	
	m_bg.LoadBitmap(IDB_BG);

	return CView::PreCreateWindow(cs);
}

// CSnakeView drawing

void CSnakeView::OnDraw(CDC* pDC)
{
	CSnakeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//GetClientRect(game_rect);

	// TO DO: FIX THE BUG
	// DEBUGGING ASSERTATION FAILED
	/*if (pDoc->current->m_bg == "DEFAULT")
	{
		m_bg.LoadBitmap(IDB_BG);
	}
	else if (pDoc->current->m_bg == "LIGHT")
	{
		m_bg.LoadBitmap(IDB_BG_LIGHT);
	}
	else if (pDoc->current->m_bg == "DARK")
	{
		m_bg.LoadBitmap(IDB_BG_DARK);
	}*/
	
	// Draw the window background
	SetRect(window_rect, 0, 0, 620 + 120, 775 + 120);
	pDC->FillRect(window_rect, &pDoc->current->bg);

	// Draw the game background
	CDC m_bgcDC;
	m_bgcDC.CreateCompatibleDC(NULL);
	m_bgcDC.SelectObject(&m_bg);
	pDC->BitBlt(BORDER, BORDER, WIDTH+BORDER, HEIGHT+BORDER, &m_bgcDC, 0, 0, SRCCOPY);

	// Draw the score region
	SetRect(score_rect, BORDER, 600+BORDER, WIDTH+BORDER, HEIGHT+BORDER*2);
	pDC->FillRect(score_rect, &pDoc->current->score);

	// Draw the food rectangle
	SetRect(food_rect, snake.food.x - 10, snake.food.y - 10,
		snake.food.x + 10, snake.food.y + 10);
	pDC->FillRect(food_rect, &pDoc->current->food);

	// Draw the snake rectangle
	for (vector<CPoint>::iterator iter = snake.snake_list.begin(); 
		iter != snake.snake_list.end(); ++iter) 
	{
		SetRect(snake_rect, (*iter).x - 10, (*iter).y - 10,
			(*iter).x + 10, (*iter).y + 10);
		pDC->FillRect(snake_rect, &pDoc->current->snake);
	}

	score.Format(_T("SCORE: %d"), (-1 + snake.snake_list.size()));
	pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
	pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	pDC->DrawText(score, -1, score_rect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
}


// CSnakeView printing

BOOL CSnakeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSnakeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSnakeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CSnakeView diagnostics

#ifdef _DEBUG
void CSnakeView::AssertValid() const
{
	CView::AssertValid();
}

void CSnakeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSnakeDoc* CSnakeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSnakeDoc)));
	return (CSnakeDoc*)m_pDocument;
}
#endif //_DEBUG


// CSnakeView message handlers


void CSnakeView::OnUp()
{
	if (snake.getDirection() == LEFT || snake.getDirection() == RIGHT)
		snake.changeDirection(UP);
}

void CSnakeView::OnDown()
{
	if (snake.getDirection() == LEFT || snake.getDirection() == RIGHT)
		snake.changeDirection(DOWN);
}

void CSnakeView::OnLeft()
{
	if (snake.getDirection() == UP || snake.getDirection() == DOWN)
		snake.changeDirection(LEFT);
}

void CSnakeView::OnRight()
{
	if (snake.getDirection() == UP || snake.getDirection() == DOWN)
		snake.changeDirection(RIGHT);
}

void CSnakeView::OnSTART()
{
	CSnakeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SetTimer(1, *pDoc->speed_current, NULL); // speed depends on nElasp ms 
	srand(time(NULL));
	snake.generateFood();
	snake.snake_list = snake.default_state;
}

void CSnakeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'W':
	case VK_UP:
		OnUp();
		break;
	case 'S':
	case VK_DOWN:
		OnDown();
		break;
	case 'A':
	case VK_LEFT:
		OnLeft();
		break;
	case 'D':
	case VK_RIGHT:
		OnRight();
		break;
	}
}


void CSnakeView::OnTimer(UINT_PTR nIDEvent)
{
	if (snake.move() != TRUE) 
	{
		KillTimer(nIDEvent);
		AfxMessageBox(_T("YOU DIED, PLEASE START OVER."));
	}
	Invalidate(FALSE);

	CView::OnTimer(nIDEvent);
}
