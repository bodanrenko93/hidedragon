// KeyGenerationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeyGeneration.h"
#include "KeyGenerationDlg.h"
#include "DlgProxy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyGenerationDlg dialog

IMPLEMENT_DYNAMIC(CKeyGenerationDlg, CDialog);

CKeyGenerationDlg::CKeyGenerationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyGenerationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyGenerationDlg)
	m_edit5 = _T("");
	m_edit6 = _T("");
	m_edit1 = _T("");
	m_edit2 = _T("");
	m_edit3 = _T("");
	m_edit4 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CKeyGenerationDlg::~CKeyGenerationDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CKeyGenerationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyGenerationDlg)
	DDX_Text(pDX, IDC_EDIT5, m_edit5);
	DDX_Text(pDX, IDC_EDIT6, m_edit6);
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT2, m_edit2);
	DDX_Text(pDX, IDC_EDIT3, m_edit3);
	DDX_Text(pDX, IDC_EDIT4, m_edit4);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeyGenerationDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyGenerationDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyGenerationDlg message handlers

BOOL CKeyGenerationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKeyGenerationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CKeyGenerationDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeyGenerationDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKeyGenerationDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CKeyGenerationDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

void CKeyGenerationDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CKeyGenerationDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CKeyGenerationDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

void CKeyGenerationDlg::OnButton1() 
{
	UpdateData();
	char cTemp1[80],cTemp2[80];
	GUID hd_guid2;
	long lRegCode[4];
	char cRegCode[4][5] = {"    ","    ","    ","    "};


/*	memset(cTemp1,0,80);
	strcpy(cTemp1,"{");
	strcat(cTemp1,m_edit1);
	strcat(cTemp1,"-");

	strcat(cTemp1,m_edit2);
	strcat(cTemp1,"-");

	strcat(cTemp1,m_edit3);
	strcat(cTemp1,"-");

	strcat(cTemp1,m_edit4);
	strcat(cTemp1,"-");

	strcat(cTemp1,m_edit5);
	strcat(cTemp1,"}");

	memset(cTemp2,0,80);
	MultiByteToWideChar(CP_ACP,MB_COMPOSITE,cTemp1,-1,(unsigned short*)cTemp2,80);

	CLSIDFromString((unsigned short*)cTemp2,(LPCLSID)&hd_guid2);



	hd_guid2.Data1 ^= 99999999;
	hd_guid2.Data2 ^= 9999;
	hd_guid2.Data3 ^= 8989;
	hd_guid2.Data4[1] ^= 6;
	hd_guid2.Data4[2] ^= 9;
	hd_guid2.Data4[3] ^= 3;
	hd_guid2.Data4[4] ^= 1;
	hd_guid2.Data4[5] ^= 6666;			
	hd_guid2.Data4[6] ^= 8;
	hd_guid2.Data4[7] ^= 8;

	memset(cTemp1,0,80);
	StringFromGUID2(hd_guid2,(unsigned short*)cTemp1,80);
	memset(cTemp2,0,80);
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,
		(unsigned short*)cTemp1,72,cTemp2,80,NULL,NULL);
	m_edit6 = cTemp2;
	
	CWnd * wnd;
	wnd = GetDlgItem(IDC_EDIT6);
	wnd->SetWindowText(m_edit6);*/

	//new method/////////////////////////////////////////////
	strcpy(cRegCode[0],(LPCSTR)m_edit1);
	strcpy(cRegCode[1],(LPCSTR)m_edit2);
	strcpy(cRegCode[2],(LPCSTR)m_edit3);
	strcpy(cRegCode[3],(LPCSTR)m_edit4);
	

	
	memset(cTemp1,0,80);
	memset(cTemp2,0,80);
	for ( int i = 0;i<4;i++)
	{
		lRegCode[i] = strtol(cRegCode[i],NULL,16);
		lRegCode[i] ^= 3333;
		lRegCode[i] |= 3333;
		memset(cTemp1,0,80);
		sprintf(cTemp1,"%X",lRegCode[i]);
		strcat(cTemp2,cTemp1);
		strcat(cTemp2,"-");
	}
	CWnd * wnd;
	wnd = GetDlgItem(IDC_EDIT6);
	wnd->SetWindowText(cTemp2);










	
}
