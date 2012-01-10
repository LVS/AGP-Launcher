#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ABPLTools.h"
#include "ABPDownloadThread.h"
#include "Resource.h"

// ABPDownload dialog

class ABPDownload : public CDialog
{
	DECLARE_DYNAMIC(ABPDownload)

public:
	ABPDownload(CWnd* pParent = NULL);   // standard constructor
	virtual ~ABPDownload();

// Dialog Data
	enum { IDD = IDD_ABPDOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
public:
	CString m_info;
	CProgressCtrl m_progress;

	CString m_basePath;	// Where ta save files
	CString m_server;	// Server
	std::vector<CString> m_downloadList;	// jar list

	int m_listIndex;
	ABPDownloadThread *m_pThread;
	bool m_bThreadExited;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
