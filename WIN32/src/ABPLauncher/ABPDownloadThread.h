#pragma once
#include "ABPLTools.h"
// ABPDownloadThread

class ABPDownloadThread : public CWinThread, public InternetFeedback
{
	DECLARE_DYNCREATE(ABPDownloadThread)

protected:
	ABPDownloadThread();           // protected constructor used by dynamic creation
	virtual ~ABPDownloadThread();

public:
	bool *m_pbFinish; // true when thread exit
	bool m_bExit;	  // to exit when user ask
	DWORD m_totalSize, m_downloaded; // UI feedback
	CString m_url, m_path; // url to download and destination path
	virtual void DownloadFeedback(DWORD totalSize, DWORD downloaded);
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run(void);
};


