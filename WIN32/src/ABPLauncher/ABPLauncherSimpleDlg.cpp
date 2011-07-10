#include "stdafx.h"
#include "ABPLauncher.h"
#include "ABPLauncherDlg.h"
#include "ABPLauncherSimpleDlg.h"
#include "ABPLTools.h"

// ABPLauncherSimpleDlg dialog

IMPLEMENT_DYNAMIC(ABPLauncherSimpleDlg, CDialog)

ABPLauncherSimpleDlg::ABPLauncherSimpleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ABPLauncherSimpleDlg::IDD, pParent)
{

}

ABPLauncherSimpleDlg::~ABPLauncherSimpleDlg()
{
}

void ABPLauncherSimpleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOSERVER, m_cboServer);
}


BEGIN_MESSAGE_MAP(ABPLauncherSimpleDlg, CDialog)
	ON_BN_CLICKED(IDC_ADVANCED, &ABPLauncherSimpleDlg::OnBnClickedAdvanced)
	ON_BN_CLICKED(IDC_LAUNCH, &ABPLauncherSimpleDlg::OnBnClickedLaunch)
END_MESSAGE_MAP()


BOOL ABPLauncherSimpleDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ABPLauncherSimpleDlg::OnPaint()
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
HCURSOR ABPLauncherSimpleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void ABPLauncherSimpleDlg::OnBnClickedAdvanced()
{
	ShowWindow(SW_HIDE);
	CABPLauncherDlg dlg2;
	INT_PTR result = dlg2.DoModal();
	if (result!=IDC_BASIC)
		EndDialog(result);
	ShowWindow(SW_SHOW);
}

void ABPLauncherSimpleDlg::OnBnClickedLaunch()
{
	UpdateData(TRUE); // Get all data from dialog
	Options opts = Options::Load();
	opts.m_lastServer.clear();
	m_cboServer.GetWindowText(opts.m_server);
	opts.m_lastServer.push_back(opts.m_server);
	for (int i=0; i<m_cboServer.GetCount(); i++)
	{
		CString server;
		m_cboServer.GetLBText(i, server); // Reader server from combo
		std::vector<CString>::iterator it = std::find(opts.m_lastServer.begin(), opts.m_lastServer.end(), server);
		if (it==opts.m_lastServer.end())
			opts.m_lastServer.push_back(server);
	}
	opts.Save();

	ABPLTools tools;
	if (tools.DownloadAndLaunch(opts))
		OnOK(); // Close launcher
}
