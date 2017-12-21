
// SnakeDoc.cpp : implementation of the CSnakeDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Snake.h"
#endif

#include "SnakeDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSnakeDoc

IMPLEMENT_DYNCREATE(CSnakeDoc, CDocument)

BEGIN_MESSAGE_MAP(CSnakeDoc, CDocument)
	ON_COMMAND(ID_THEME_LIGHT, &CSnakeDoc::OnThemeLight)
	ON_COMMAND(ID_THEME_DARK, &CSnakeDoc::OnThemeDark)
	ON_COMMAND(ID_THEME_DEFAULT, &CSnakeDoc::OnThemeDefault)
	ON_UPDATE_COMMAND_UI(ID_THEME_DEFAULT, &CSnakeDoc::OnUpdateThemeDefault)
	ON_UPDATE_COMMAND_UI(ID_THEME_LIGHT, &CSnakeDoc::OnUpdateThemeLight)
	ON_UPDATE_COMMAND_UI(ID_THEME_DARK, &CSnakeDoc::OnUpdateThemeDark)
	ON_COMMAND(ID_SPEED_LOW, &CSnakeDoc::OnSpeedLow)
	ON_COMMAND(ID_SPEED_DEFAULT, &CSnakeDoc::OnSpeedDefault)
	ON_COMMAND(ID_SPEED_HIGH, &CSnakeDoc::OnSpeedHigh)
	ON_UPDATE_COMMAND_UI(ID_SPEED_LOW, &CSnakeDoc::OnUpdateSpeedLow)
	ON_UPDATE_COMMAND_UI(ID_SPEED_DEFAULT, &CSnakeDoc::OnUpdateSpeedDefault)
	ON_UPDATE_COMMAND_UI(ID_SPEED_HIGH, &CSnakeDoc::OnUpdateSpeedHigh)
END_MESSAGE_MAP()


// CSnakeDoc construction/destruction

CSnakeDoc::CSnakeDoc()
{
	// Define the light theme
	light.bg.CreateSolidBrush(RGB(40, 160, 40));
	light.score.CreateSolidBrush(RGB(80, 46, 80));
	light.food.CreateSolidBrush(RGB(252, 70, 60));
	light.snake.CreateSolidBrush(RGB(40, 142, 228));

	// Define the dark theme
	dark.bg.CreateSolidBrush(RGB(40, 160, 40));
	dark.score.CreateSolidBrush(RGB(80, 46, 80));
	dark.food.CreateSolidBrush(RGB(38, 46, 49));
	dark.snake.CreateSolidBrush(RGB(80, 46, 80));

	// Define the default theme
	defalt.bg.CreateSolidBrush(RGB(40, 160, 40));
	defalt.score.CreateSolidBrush(RGB(120, 160, 40));
	defalt.food.CreateSolidBrush(RGB(255, 80, 80));
	defalt.snake.CreateSolidBrush(RGB(51, 102, 255));

	current = &defalt;

	// Smaller means higher speed
	speed_low = 150;
	speed_default = 100;
	speed_high = 50;
	speed_current = &speed_default;

}

CSnakeDoc::~CSnakeDoc()
{
}

BOOL CSnakeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSnakeDoc serialization

void CSnakeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSnakeDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSnakeDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSnakeDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSnakeDoc diagnostics

#ifdef _DEBUG
void CSnakeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSnakeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSnakeDoc commands


void CSnakeDoc::OnThemeLight()
{
	current = &light;

}


void CSnakeDoc::OnThemeDark()
{
	current = &dark;

}


void CSnakeDoc::OnThemeDefault()
{
	current = &defalt;
	
}


void CSnakeDoc::OnUpdateThemeDefault(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(current == &defalt);
}


void CSnakeDoc::OnUpdateThemeLight(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(current == &light);
}


void CSnakeDoc::OnUpdateThemeDark(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(current == &dark);
}


void CSnakeDoc::OnSpeedLow()
{
	speed_current = &speed_low;
}


void CSnakeDoc::OnSpeedDefault()
{
	speed_current = &speed_default;
}


void CSnakeDoc::OnSpeedHigh()
{
	speed_current = &speed_high;
}


void CSnakeDoc::OnUpdateSpeedLow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(speed_current == &speed_low);
}


void CSnakeDoc::OnUpdateSpeedDefault(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(speed_current == &speed_default);
}


void CSnakeDoc::OnUpdateSpeedHigh(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(speed_current == &speed_high);
}


