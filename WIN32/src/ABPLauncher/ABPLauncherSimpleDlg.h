#pragma once
#include "afxwin.h"


// ABPLauncherSimpleDlg dialog

class ABPLauncherSimpleDlg : public CDialog
{
	DECLARE_DYNAMIC(ABPLauncherSimpleDlg)

public:
	ABPLauncherSimpleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ABPLauncherSimpleDlg();

// Dialog Data
	enum { IDD = IDD_ABPLAUNCHERSIMPLEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cboServer;
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedAdvanced();
	afx_msg void OnBnClickedLaunch();
};
