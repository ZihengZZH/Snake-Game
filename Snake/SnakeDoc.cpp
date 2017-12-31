
// SnakeDoc.cpp : implementation of the CSnakeDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Snake.h"
#endif

#include "SnakeDoc.h"
#include "SnakeView.h"
#include "CSnake.h"
#include <propkey.h>

#include <windows.h>
#include <MsXml6.h>
#include <comutil.h>
#pragma comment(lib, "comsuppwd.lib")

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
	// game rect 217/220/209
	light.bg.CreateSolidBrush(RGB(184, 171, 165));
	light.score.CreateSolidBrush(RGB(80, 46, 80));
	light.food.CreateSolidBrush(RGB(255, 237, 131));
	light.snake.CreateSolidBrush(RGB(100, 243, 120));
	light.snake_head.CreateSolidBrush(RGB(92, 224, 110));
	light.m_bg = "LIGHT";

	// Define the dark theme
	// game rect 40/28/45
	dark.bg.CreateSolidBrush(RGB(105, 94, 147));
	dark.score.CreateSolidBrush(RGB(80, 46, 80));
	dark.food.CreateSolidBrush(RGB(190, 175, 194));
	dark.snake.CreateSolidBrush(RGB(129, 85, 186));
	dark.snake_head.CreateSolidBrush(RGB(150, 110, 176));
	dark.m_bg = "DARK";

	// Define the default theme
	// Classic green background with blue snake and red food
	// game rect 51/204/51
	defalt.bg.CreateSolidBrush(RGB(40, 160, 40));
	defalt.score.CreateSolidBrush(RGB(120, 160, 40));
	defalt.food.CreateSolidBrush(RGB(255, 80, 80));
	defalt.snake.CreateSolidBrush(RGB(51, 102, 255));
	defalt.snake_head.CreateSolidBrush(RGB(36, 120, 190));
	defalt.m_bg = "DEFAULT";

	current = &defalt;

	// Smaller means higher speed
	speed_low = 250;
	speed_default = 150;
	speed_high = 50;
	speed_current = &speed_default;
	
}


CSnakeDoc::~CSnakeDoc()
{
}


UINT CSnakeDoc::RetrieveHighest()
{
	UINT m_old;

	CoInitialize(NULL);
	CComPtr<IXMLDOMDocument> spXmldoc;
	HRESULT hr = spXmldoc.CoCreateInstance(L"MSXML2.DOMDocument.6.0");

	if (SUCCEEDED(hr))
	{
		VARIANT_BOOL isSuccessful;
		CComVariant varXmlFile(L"database.xml");

		spXmldoc->put_async(VARIANT_FALSE);
		HRESULT hr = spXmldoc->load(varXmlFile, &isSuccessful);

		if (isSuccessful == VARIANT_TRUE)
		{
			CComBSTR bstrXml;
			CComPtr<IXMLDOMElement> spRoot = NULL;
			CComPtr<IXMLDOMElement> spRecord = NULL;
			CComPtr<IXMLDOMElement> spScore = NULL;
			CComPtr<IXMLDOMNode> spTheNode = NULL;

			// KEY TO INCLUDE THE ROOT
			hr = spXmldoc->get_documentElement(&spRoot);
			spRoot->get_xml(&bstrXml);

			if (speed_current == &speed_low)
				spRoot->selectSingleNode(L"/record/highest[@id='low']", &spTheNode);
			else if (speed_current == &speed_default)
				spRoot->selectSingleNode(L"/record/highest[@id='default']", &spTheNode);
			else
				spRoot->selectSingleNode(L"/record/highest[@id='high']", &spTheNode);

			hr = spTheNode.QueryInterface(&spRecord);
			spTheNode.Release();

			spRecord->get_xml(&bstrXml);

			CComPtr<IXMLDOMNodeList> spNodeList = NULL;
			CComPtr<IXMLDOMNode> spListItem = NULL;
			spRecord->get_childNodes(&spNodeList);
			spNodeList->get_item(1, &spListItem);

			if (spListItem)
			{
				spListItem->get_xml(&bstrXml);

				VARIANT value;
				hr = spListItem->get_nodeTypedValue(&value);
				if (FAILED(hr))
					throw "failed";

				if (hr == S_OK)
				{
					m_old = 0;
					if (value.vt == VT_BSTR)
					{
						CString strValue = (_bstr_t)value;
						m_old = _ttoi((LPCTSTR)strValue);
					}
				}
			}
			spRecord.Release();
			spRoot.Release();
			bstrXml.Empty();
		}
		varXmlFile.Clear();
	}

	spXmldoc.Release();
	CoUninitialize();

	return m_old;
}


void CSnakeDoc::UpdateDatabase()
{
	CoInitialize(NULL);
	CComPtr<IXMLDOMDocument> spXmldoc;
	HRESULT hr = spXmldoc.CoCreateInstance(L"MSXML2.DOMDocument.6.0");

	if (SUCCEEDED(hr))
	{
		VARIANT_BOOL isSuccessful;
		CComVariant varXmlFile(L"database.xml");

		spXmldoc->put_async(VARIANT_FALSE);
		HRESULT hr = spXmldoc->load(varXmlFile, &isSuccessful);

		if (isSuccessful == VARIANT_TRUE)
		{
			CComBSTR bstrXml;
			CComPtr<IXMLDOMElement> spRoot = NULL;
			CComPtr<IXMLDOMElement> spRecord = NULL;
			CComPtr<IXMLDOMElement> spScore = NULL;
			CComPtr<IXMLDOMNode> spTheNode = NULL;

			// KEY TO INCLUDE THE ROOT
			hr = spXmldoc->get_documentElement(&spRoot);
			spRoot->get_xml(&bstrXml);

			if (speed_current == &speed_low)
				spRoot->selectSingleNode(L"/record/highest[@id='low']", &spTheNode);
			else if (speed_current == &speed_default)
				spRoot->selectSingleNode(L"/record/highest[@id='default']", &spTheNode);
			else
				spRoot->selectSingleNode(L"/record/highest[@id='high']", &spTheNode);

			hr = spTheNode.QueryInterface(&spRecord);
			spTheNode.Release();

			spRecord->get_xml(&bstrXml);

			CComPtr<IXMLDOMNodeList> spNodeList = NULL;
			CComPtr<IXMLDOMNode> spListItem = NULL;
			CComPtr<IXMLDOMNode> spListTime = NULL;
			spRecord->get_childNodes(&spNodeList);
			spNodeList->get_item(1, &spListItem);
			spNodeList->get_item(2, &spListTime);

			if (spListItem)
			{
				spListItem->get_xml(&bstrXml);

				VARIANT value;
				hr = spListItem->get_nodeTypedValue(&value);
				if (FAILED(hr))
					throw "failed";

				if (hr == S_OK)
				{
					UINT m_old = 0;
					if (value.vt == VT_BSTR)
					{
						CString strValue = (_bstr_t)value;
						m_old = _ttoi((LPCTSTR)strValue);
					}
					UINT m_new = new_highest;
					if (m_new > m_old)
					{
						CString m_new_str, m_time_str;
						m_new_str.Format(_T("%u"), m_new);
						spListItem->put_text(m_new_str.AllocSysString());
						SYSTEMTIME sys;
						GetLocalTime(&sys);
						m_time_str.Format(_T("%4d/%02d/%02d %02d:%02d:%02d.%03d"), 
							sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
						spListTime->put_text(m_time_str.AllocSysString());
					}
				}
			}

			spRecord.Release();
			spRoot.Release();
			bstrXml.Empty();

			spXmldoc->save(varXmlFile);
		}
		varXmlFile.Clear();
	}

	spXmldoc.Release();
	CoUninitialize();
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
	CFrameWndEx *pMain = (CFrameWndEx *)AfxGetMainWnd();
	CSnakeView *pView = (CSnakeView *)pMain->GetActiveView();

	if (ar.IsStoring())
	{
		pView->snake.Serialize(ar);
	}
	else
	{
		pView->snake.Serialize(ar);
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


/*
void CSnakeDoc::OnGameSave()
{
	CFileException fe;
	CFile *pFile = GetFile(_T("database.txt"),
		CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &fe);
	DeleteContents();
	SetModifiedFlag();

	CArchive saveArchive(pFile, CArchive::store);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = FALSE;

	CWaitCursor wait;
	if (pFile->GetLength() != 0)
		Serialize(saveArchive);

	saveArchive.Close();
	ReleaseFile(pFile, FALSE);
}


void CSnakeDoc::OnGameOpen()
{
	CFileException fe;
	CFile *pFile = GetFile(_T("database.txt"),
		CFile::modeRead | CFile::shareDenyWrite, &fe);
	DeleteContents();
	SetModifiedFlag();

	CArchive loadArchive(pFile, CArchive::load);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;

	CWaitCursor wait;
	if (pFile->GetLength() != 0)
		Serialize(loadArchive);

	loadArchive.Close();
	ReleaseFile(pFile, FALSE);
}
*/