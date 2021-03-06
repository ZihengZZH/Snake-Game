
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
#include "afxwin.h"

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
	ON_COMMAND(ID_GAME_PAUSE, &CSnakeView::OnGamePause)
	ON_COMMAND(ID_GAME_CONTINUE, &CSnakeView::OnGameContinue)
	ON_COMMAND(ID_GAME_STOP, &CSnakeView::OnGameStop)
END_MESSAGE_MAP()

// CSnakeView construction/destruction

CSnakeView::CSnakeView()
{
	m_score = 0;
	m_highest = 0;
	m_pause = FALSE;
	m_died = TRUE;
}

CSnakeView::~CSnakeView()
{
}

BOOL CSnakeView::PreCreateWindow(CREATESTRUCT& cs)
{
	
	// All the image attributes load images
	m_bg.LoadBitmap(IDB_BG);
	m_bg_light.LoadBitmap(IDB_BG_LIGHT);
	m_bg_dark.LoadBitmap(IDB_BG_DARK);
	// Absolute path
	m_food.Load(_T("..\\Snake\\res\\apple.png"));
	m_trophy.Load(_T("..\\Snake\\res\\trophy.png"));

	// Set the font for score display
	m_font.CreatePointFont(180, L"Century Gothic"); // Small Fonts
	m_info_font.CreatePointFont(220, L"Arial");

	m_count = 0;

	return CView::PreCreateWindow(cs);
}

// CSnakeView drawing

void CSnakeView::OnDraw(CDC* pDC)
{
	CSnakeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Double buffering technique
	CDC m_cacheDC;
	CBitmap m_cacheCBitmap;

	//GetClientRect(&game_rect);
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(pDC, 620 + 120, 775 + 50);
	
	CBitmap *pOldBit = m_cacheDC.SelectObject(&m_cacheCBitmap);

	// Draw the window background
	SetRect(window_rect, 0, 0, 620 + 120, 775 + 120);
	m_cacheDC.FillRect(window_rect, &pDoc->current->bg);
	/*pDC->FillRect(window_rect, &pDoc->current->bg);*/

	// Draw the game background
	CDC m_bgcDC;
	// Choose the theme background
	m_bgcDC.CreateCompatibleDC(NULL);
	if (pDoc->current->m_bg == "DEFAULT")
		m_bgcDC.SelectObject(&m_bg);
	else if (pDoc->current->m_bg == "LIGHT")
		m_bgcDC.SelectObject(&m_bg_light);
	else if (pDoc->current->m_bg == "DARK")
		m_bgcDC.SelectObject(&m_bg_dark);
	m_cacheDC.BitBlt(BORDER, BORDER, WIDTH + BORDER, HEIGHT + BORDER, &m_bgcDC, 0, 0, SRCCOPY);
	/*pDC->BitBlt(BORDER, BORDER, WIDTH+BORDER, HEIGHT+BORDER, &m_bgcDC, 0, 0, SRCCOPY);*/
	
	if (!m_died)
	{
		// Draw the food rectangle
		SetRect(food_rect, snake.food.x - 10, snake.food.y - 10,
			snake.food.x + 10, snake.food.y + 10);
		/*m_food.Draw(*pDC, food_rect);*/
		m_cacheDC.FillRect(food_rect, &pDoc->current->food);
		/*pDC->FillRect(food_rect, &pDoc->current->food);*/

		// Draw the snake rectangle
		for (vector<CPoint>::iterator iter = snake.snake_list.begin();
			iter != snake.snake_list.end(); ++iter)
		{
			SetRect(snake_rect, (*iter).x - 10, (*iter).y - 10,
				(*iter).x + 10, (*iter).y + 10);
			if (iter == snake.snake_list.begin())
				m_cacheDC.FillRect(snake_rect, &pDoc->current->snake_head);
			else
				m_cacheDC.FillRect(snake_rect, &pDoc->current->snake);
			/*pDC->FillRect(snake_rect, &pDoc->current->snake);*/
		}
	}
	else
	{
		// Information for the user at the beginning
		SetRect(info_rect,80,200,640,350);
		info.Format(_T("PLEASE PRESS NEW GAME TO START\nMore info refers to HELP"));
		m_cacheDC.SelectObject(m_info_font);
		//CBrush info_brush(RGB(50, 100, 255));
		//m_cacheDC.FillRect(info_rect, &info_brush);
		m_cacheDC.SetBkMode(TRANSPARENT);
		m_cacheDC.SetTextColor(RGB(255, 255, 255));
		m_cacheDC.DrawText(info, -1, info_rect, DT_CENTER | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	}

	m_score = -3 + snake.snake_list.size();
	
	// Draw the score region
	SetRect(score_rect, BORDER*1.5, 0, WIDTH/4, BORDER);
	SetRect(highest_rect, BORDER*2.5, 0, WIDTH/2, BORDER);
	SetRect(level_rect, BORDER*3, 0, WIDTH, BORDER);
	SetRect(m_food_rect, BORDER, 5, BORDER+40, 50);
	SetRect(m_trophy_rect, BORDER+100, 10, BORDER+140, 50);
	m_food.Draw(m_cacheDC, m_food_rect);
	m_trophy.Draw(m_cacheDC, m_trophy_rect);
	/*m_food.Draw(*pDC, m_food_rect);
	m_trophy.Draw(*pDC, m_trophy_rect);*/
	/*
	There are still problems with the icon that will flicker.
	Sometimes it might affect user experience.
	*/
	
	// Transparent the background of text
	m_cacheDC.SetBkMode(TRANSPARENT);
	/*pDC->SetBkMode(TRANSPARENT);*/

	// Score string
	score.Format(_T("%d"), m_score);
	/*pDC->SelectObject(GetStockObject(SYSTEM_FONT));
	//pDC->SelectObject(m_font);
	//pDC->SetTextColor(RGB(255,255,255));
	pDC->DrawText(score, -1, score_rect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);*/
	m_cacheDC.SelectObject(m_font);
	m_cacheDC.SetTextColor(RGB(255, 255, 255));
	m_cacheDC.DrawText(score, -1, score_rect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);

	// Highest score string
	highest.Format(_T("%d"), m_highest);
	/*pDC->SelectObject(m_font);
	//pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText(highest, -1, highest_rect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);*/
	m_cacheDC.SelectObject(m_font);
	m_cacheDC.SetTextColor(RGB(255, 255, 255));
	m_cacheDC.DrawText(highest, -1, highest_rect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);

	// LevelUp string
	if (snake.level_up || m_count > 0)
	{
		level.Format(_T("LEVEL UP! SPEED UP!"));
		m_cacheDC.SelectObject(m_font);
		m_cacheDC.SetTextColor(RGB(255, 255, 255));
		m_cacheDC.DrawText(level, -1, level_rect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
		m_count++;
		if (m_count == 10)
			m_count = 0;
	}
	
	// Draw the cacheDC to the window
	pDC->BitBlt(0, 0, 620 + 120, 775 + 120, &m_cacheDC, 0, 0, SRCCOPY);

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


// START the game the set the timer
void CSnakeView::OnSTART()
{
	CSnakeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_highest = pDoc->RetrieveHighest();
	snake.speed = *pDoc->speed_current;
	m_died = FALSE;

	CString bgm = _T("..\\Snake\\res\\bgm.wav");
	hMCI = MCIWndCreate(NULL, NULL, WS_POPUP | MCIWNDF_NOPLAYBAR | MCIWNDF_NOMENU, bgm);
	MCIWndPlay(hMCI);
	//PlaySound(L".\\res\\bgm.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//mciSendString(_T("open res\\bgm.wav alias aa"), NULL, 0, NULL);
	SetTimer(1, snake.speed, NULL); // speed depends on nElasp ms 
	srand(time(NULL));
	snake.generateFood();
	snake.snake_list = snake.default_state;
}


// Respond to the key press
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


// STOP the game and kill the timer
void CSnakeView::OnTimer(UINT_PTR nIDEvent)
{
	CSnakeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (snake.is_food)
	{
		PlaySound(L"..\\Snake\\res\\food.wav", NULL, SND_FILENAME | SND_ASYNC);
		//mciSendString(_T("open res\\food.wav alias aa"), NULL, 0, NULL);
	}

	if (snake.level_up)
	{
		KillTimer(nIDEvent);
		SetTimer(nIDEvent, snake.speed - snake.level * 5, NULL);
	}

	if (snake.move() != TRUE) 
	{
		MCIWndDestroy(hMCI);
		PlaySound(L"..\\Snake\\res\\died.wav", NULL, SND_FILENAME | SND_ASYNC);
		//mciSendString(_T("open res\\died.wav alias aa"), NULL, 0, NULL);

		KillTimer(nIDEvent);
		m_died = TRUE;
		pDoc->new_highest = m_score;
		pDoc->new_level = snake.level;
		pDoc->UpdateDatabase();
		//AfxMessageBox(_T("YOU DIED, PLEASE START OVER."));
	}
	Invalidate(FALSE);

	CView::OnTimer(nIDEvent);
}


// Pause the game and kill the timer
void CSnakeView::OnGamePause()
{
	m_pause = TRUE;
	KillTimer(1);
}


// Continue the game and set the timer
void CSnakeView::OnGameContinue()
{
	if (m_pause)
		SetTimer(1, snake.speed, NULL);
}


// Stop the game and kill the timer
void CSnakeView::OnGameStop()
{
	MCIWndDestroy(hMCI);
	PlaySound(L"..\\Snake\\res\\died.wav", NULL, SND_FILENAME | SND_ASYNC);
	m_pause = FALSE;
	KillTimer(1);

	CSnakeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->new_highest = m_score;
	pDoc->UpdateDatabase();

	//AfxMessageBox(_T("GAME STOPPED, PLEASE START OVER."));
}

