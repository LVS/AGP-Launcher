
// ABPLauncher.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ABPLauncher.h"
#include "ABPLauncherSimpleDlg.h"
#include "ABPLTools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CABPLauncherApp

BEGIN_MESSAGE_MAP(CABPLauncherApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CABPLauncherApp construction

CABPLauncherApp::CABPLauncherApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CABPLauncherApp object

CABPLauncherApp theApp;


// CABPLauncherApp initialization

BOOL CABPLauncherApp::InitInstance()
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

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("ABP Launcher"));

	ABPLTools tools;
	CString java = tools.GetJavawExePath();
	if (java.GetLength()==0)
	{
		if (::AfxMessageBox("You need to install Java.\r\nDo you want to go to Java download site ?", MB_YESNO)==IDYES)
			ShellExecute(NULL, "open", "http://www.java.com", NULL, NULL, SW_SHOW);
	}

	ABPLauncherSimpleDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}
