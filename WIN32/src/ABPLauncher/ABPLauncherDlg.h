
// ABPLauncherDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ABPDownload.h"
#include "ABPLTools.h"

// CABPLauncherDlg dialog
class CABPLauncherDlg : public CDialog
{
// Construction
public:
	CABPLauncherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ABPLAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:

	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// Server list combobox
	CComboBox m_cboServer;
	afx_msg void OnBnClickedOk();
	CAnimateCtrl m_copyAnimation;
	CComboBox m_language;
	CComboBox m_memory;
	CComboBox m_application;
	CString m_customClass;
	BOOL m_debug;
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedBasic();
};
