
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
	ON_COMMAND(ID_KEY_KEYCONTROL, &CSnakeApp::OnKeyAbout)
	ON_COMMAND(ID_HELP_RULE, &CSnakeApp::OnRuleAbout)
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


// CKeyDlg dialog used for Button Control

class CKeyDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(CKeyDlg)

public:
	enum { IDD = IDD_KEYBOX };
	CKeyDlg();
	virtual ~CKeyDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_up;
	CButton m_down;
	CButton m_left;
	CButton m_right;
	afx_msg void OnClickedDown();
	afx_msg void OnClickedLeft();
	afx_msg void OnClickedRight();
	afx_msg void OnClickedUp();
	BOOL OnInitDialog();
	void OnPaint();
};

CKeyDlg::CKeyDlg() : CDialogEx(IDD_KEYBOX)
{
}

CKeyDlg::~CKeyDlg()
{
}

void CKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_DOWN, m_down);
	DDX_Control(pDX, IDC_LEFT, m_left);
	DDX_Control(pDX, IDC_RIGHT, m_right);
}

BOOL CKeyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CFont m_font;
	m_font.CreateFont(50, 0, 0, 0, FW_BOLD, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0, _T("Arial"));
	m_up.SetFont(&m_font);
	m_down.SetFont(&m_font);
	m_left.SetFont(&m_font);
	m_right.SetFont(&m_font);

	return TRUE;
}

void CKeyDlg::OnPaint()
{
}

BEGIN_MESSAGE_MAP(CKeyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_DOWN, &CKeyDlg::OnClickedDown)
	ON_BN_CLICKED(IDC_LEFT, &CKeyDlg::OnClickedLeft)
	ON_BN_CLICKED(IDC_RIGHT, &CKeyDlg::OnClickedRight)
	ON_BN_CLICKED(IDC_UP, &CKeyDlg::OnClickedUp)
END_MESSAGE_MAP()


void CKeyDlg::OnClickedDown()
{
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	if (pFrame)
	{
		CSnakeView *pView = (CSnakeView*)pFrame->GetActiveView();
		if (pView)
		{
			pView->OnDown();
		}
	}
}


void CKeyDlg::OnClickedLeft()
{
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	if (pFrame)
	{
		CSnakeView *pView = (CSnakeView*)pFrame->GetActiveView();
		if (pView)
		{
			pView->OnLeft();
		}
	}
}


void CKeyDlg::OnClickedRight()
{
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	if (pFrame)
	{
		CSnakeView *pView = (CSnakeView*)pFrame->GetActiveView();
		if (pView)
		{
			pView->OnRight();
		}
	}
}


void CKeyDlg::OnClickedUp()
{
	CMainFrame *pFrame = (CMainFrame*)::AfxGetMainWnd();
	if (pFrame)
	{
		CSnakeView *pView = (CSnakeView*)pFrame->GetActiveView();
		if (pView)
		{
			pView->OnUp();
		}
	}
}


// CRuleDlg dialog used for Game Rule

class CRuleDlg : public CDialogEx
{
	//DECLARE_DYNAMIC(CRuleDlg)

public:
	enum { IDD = IDD_RULE };
	CRuleDlg();
	virtual ~CRuleDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	void OnPaint();

	DECLARE_MESSAGE_MAP()
};

CRuleDlg::CRuleDlg() : CDialogEx(IDD_RULE)
{
}

CRuleDlg::~CRuleDlg()
{
}

void CRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CRuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CFont m_big_font, m_small_font;
	m_big_font.CreateFont(20, 0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0, _T("Arial"));
	m_small_font.CreateFont(10, 0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0, _T("Arial"));
	GetDlgItem(IDC_RULE)->SetFont(&m_big_font);
	GetDlgItem(IDC_RULE1)->SetFont(&m_small_font);
	GetDlgItem(IDC_RULE2)->SetFont(&m_small_font);
	GetDlgItem(IDC_RULE3)->SetFont(&m_small_font);
	GetDlgItem(IDC_RULE4)->SetFont(&m_small_font);
	
	return TRUE;
}

void CRuleDlg::OnPaint()
{
	/*	NOT USED  */

	HWND hwnd = GetSafeHwnd();
	::InvalidateRect(hwnd, NULL, true);
	::UpdateWindow(hwnd);

	CDC *pDC = GetDC();
	CImage image;
	image.Load(_T("./res/Rule.png"));
	if (image.IsNull())
	{
		return;
	}
	if (image.GetBPP() == 32)
	{
		int i, j;
		for (i = 0; i < image.GetWidth(); i++)
		{
			for (j = 0; j < image.GetHeight(); j++)
			{
				byte *pByte = (byte *)image.GetPixelAddress(i, j);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}
	CDC m_cacheDC;
	CBitmap m_cacheCBitmap;
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(pDC, 456, 212);
	CBitmap *pOldBit = m_cacheDC.SelectObject(&m_cacheCBitmap);

	image.Draw(pDC->m_hDC, 0, 0);
	image.Draw(m_cacheDC, 0, 0);
	pDC->BitBlt(0, 0, 456, 212, &m_cacheDC, 0, 0, SRCCOPY);
	image.Destroy();
	ReleaseDC(pDC);

}

BEGIN_MESSAGE_MAP(CRuleDlg, CDialogEx)
END_MESSAGE_MAP()



// App command to run the dialog
void CSnakeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// App command to run the dialog
void CSnakeApp::OnKeyAbout()
{
	CKeyDlg keyDlg;
	keyDlg.DoModal();
}

// App command to run the dialog
void CSnakeApp::OnRuleAbout()
{
	CRuleDlg ruleDlg;
	ruleDlg.DoModal();
}



// CSnakeApp message handlers



