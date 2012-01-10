#include "stdafx.h"
#include "ABPLauncher.h"
#include "ABPLauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CABPLauncherDlg::CABPLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CABPLauncherDlg::IDD, pParent)
	, m_customClass(_T(""))
	, m_debug(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CABPLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOSERVER, m_cboServer);
	DDX_Control(pDX, IDC_COMBO1, m_language);
	DDX_Control(pDX, IDC_COMBO2, m_memory);
	DDX_Control(pDX, IDC_COMBO3, m_application);
	DDX_Text(pDX, IDC_EDIT1, m_customClass);
	DDX_Check(pDX, IDC_CHECK1, m_debug);
}

BEGIN_MESSAGE_MAP(CABPLauncherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CABPLauncherDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLEAR, &CABPLauncherDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_BASIC, &CABPLauncherDlg::OnBnClickedBasic)
END_MESSAGE_MAP()


// CABPLauncherDlg message handlers

BOOL CABPLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	Options opts = Options::Load();

	for (int i=0; i<(int)opts.m_lastServer.size(); i++)
		m_cboServer.AddString(opts.m_lastServer[i]);
	m_cboServer.SelectString(0, opts.m_server);

	m_application.AddString("Toolbar");
	m_application.AddString("TMS");
	m_application.AddString("Retail Monitor");
	m_application.AddString("TRNI Feed Monitor");
	m_application.SetCurSel(opts.m_application);

	m_language.AddString("English");
	m_language.AddString("French");
	m_language.AddString("Hebrew");
	m_language.SetCurSel(opts.m_language);

	m_memory.AddString("256");
	m_memory.AddString("512");
	m_memory.AddString("1024");
	m_memory.SetCurSel(opts.m_memory);

	m_customClass = opts.m_class;

	m_debug = opts.m_debug==1;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CABPLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CABPLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CABPLauncherDlg::OnBnClickedOk()
{
	UpdateData(TRUE); // Get all data from dialog
	Options opts;
	m_cboServer.GetWindowText(opts.m_server);
	opts.m_lastServer.push_back(opts.m_server);
	for (int i=0; i<m_cboServer.GetCount(); i++)
	{
		CString server;
		m_cboServer.GetLBText(i, server); 
		std::vector<CString>::iterator it = std::find(opts.m_lastServer.begin(), opts.m_lastServer.end(), server);
		if (it==opts.m_lastServer.end())
			opts.m_lastServer.push_back(server);
	}
	opts.m_language = m_language.GetCurSel();
	opts.m_memory = m_memory.GetCurSel();
	opts.m_application = m_application.GetCurSel();
	opts.m_class = m_customClass;
	opts.m_debug = m_debug?1:0;
	opts.Save();

	ABPLTools tools;
	if (tools.DownloadAndLaunch(opts))
		OnOK(); // Close launcher
}

void CABPLauncherDlg::OnBnClickedClear()
{
	Options opts = Options::Load();
	ABPLTools tools;
	tools.RemoveDirectory(tools.GetWorkingFolder(), false, false);
	opts.Save();
}

void CABPLauncherDlg::OnBnClickedBasic()
{
	EndDialog(IDC_BASIC);
}