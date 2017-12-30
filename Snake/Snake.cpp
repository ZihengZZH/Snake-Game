
// Snake.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Snake.h"
#include "MainFrm.h"

#include "SnakeDoc.h"
#include "SnakeView.h"
#include "afxwin.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSnakeApp

BEGIN_MESSAGE_MAP(CSnakeApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSnakeApp::OnAppAbout)
	ON_COMMAND(ID_GAME_LEADERBOARD, &CSnakeApp::OnLeaderAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CSnakeApp construction

CSnakeApp::CSnakeApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Snake.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CSnakeApp object

CSnakeApp theApp;


// CSnakeApp initialization

BOOL CSnakeApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSnakeDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSnakeView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Change the name of window
	(AfxGetMainWnd())->SetWindowText(_T("Snake Game"));

	// Change the icon of window
	HICON hIcon = LoadIcon(IDR_MYICON);
	HICON hPrev = m_pMainWnd->SetIcon(hIcon, FALSE);
	if (hPrev != NULL && hPrev != hIcon)
		DestroyIcon(hPrev);

	return TRUE;
}

int CSnakeApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CSnakeApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(CAboutDlg)

public:
	enum { IDD = IDD_ABOUTBOX };
	CAboutDlg();
	virtual ~CAboutDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLeaderDlg dialog used for Leader Board About

class CLeaderDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(CLeaderDlg)

public:
	enum { IDD = IDD_LEADERBOX };
	CLeaderDlg();
	virtual ~CLeaderDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	void OnPaint();
	void RetrieveDatabase();

	DECLARE_MESSAGE_MAP()
public:
	CFont font_new;
	CBrush m_brush;
	CImage m_trophy;
	CRect m_record_rect;
	CString m_level;
	CString m_score;
	CString m_time;
	std::vector<CString> m_records;
};

CLeaderDlg::CLeaderDlg() : CDialogEx(IDD_LEADERBOX)
{
}

CLeaderDlg::~CLeaderDlg()
{
}

void CLeaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CLeaderDlg::OnInitDialog()
{
	font_new.CreatePointFont(50, L"Century Gothic");
	m_trophy.Load(_T("res//trophy.png"));
	m_brush.CreateSolidBrush(RGB(170, 240, 170));
	RetrieveDatabase();

	return TRUE;
}

void CLeaderDlg::OnPaint()
{
	CPaintDC dc(this);
	SendMessage(WM_ICONERASEBKGND, 
		reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		
	CRect rect, trophy_rect;
	GetClientRect(&rect);
	UINT pivot = rect.Width() / 2;
	SetRect(trophy_rect, pivot - 30, 5, pivot + 30, 65);
	
	dc.FillRect(rect, &m_brush);
	m_trophy.Draw(dc, trophy_rect);

	for (auto i = 0; i != 3; i++)
	{
		SetRect(m_record_rect, pivot - 50, 100 + 50 * i, pivot + 50, 140 + 50 * i);
		m_level.Format(_T("LEVEL: %d"), m_records[i]);
		m_score.Format(_T("SCORE: %d"), m_records[i+1]);
		m_time.Format(_T("TIME: %d"), m_records[i+2]);
		dc.SelectObject(font_new);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(50, 50, 80));
		dc.DrawText(m_level, -1, m_record_rect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_TOP | DT_END_ELLIPSIS);
		dc.DrawText(m_score, -1, m_record_rect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
		dc.DrawText(m_time, -1, m_record_rect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_BOTTOM | DT_END_ELLIPSIS);
	}


}

void CLeaderDlg::RetrieveDatabase()
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
			CComPtr<IXMLDOMNode> spTheNode = NULL;

			// KEY TO INCLUDE THE ROOT
			hr = spXmldoc->get_documentElement(&spRoot);
			spRoot->get_xml(&bstrXml);

			for (int j = 0; j != 3; j++)
			{
				if (j == 0)
					spRoot->selectSingleNode(L"/record/highest[@id='low']", &spTheNode);
				else if (j == 1)
					spRoot->selectSingleNode(L"/record/highest[@id='default']", &spTheNode);
				else
					spRoot->selectSingleNode(L"/record/highest[@id='high']", &spTheNode);

				hr = spTheNode.QueryInterface(&spRecord);
				spTheNode.Release();

				spRecord->get_xml(&bstrXml);

				for (auto i = 0; i != 3; i++)
				{
					CComPtr<IXMLDOMNodeList> spNodeList = NULL;
					spRecord->get_childNodes(&spNodeList);
					CComPtr<IXMLDOMNode> spListItem = NULL;
					spNodeList->get_item(i, &spListItem);

					if (spListItem)
					{
						spListItem->get_xml(&bstrXml);

						VARIANT value;
						hr = spListItem->get_nodeTypedValue(&value);
						if (FAILED(hr))
							throw "failed";

						if (hr == S_OK)
						{
							CString s;
							s = (CString)value.bstrVal;
							VariantClear(&value);
							m_records.push_back(s);
						}
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
}

BEGIN_MESSAGE_MAP(CLeaderDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// App command to run the dialog
void CSnakeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CSnakeApp::OnLeaderAbout()
{
	CLeaderDlg leaderDlg;
	leaderDlg.DoModal();
}


// CSnakeApp message handlers



