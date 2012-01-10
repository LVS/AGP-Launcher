// ABPDownloadThread.cpp : implementation file
//

#include "stdafx.h"
#include "ABPLauncher.h"
#include "ABPDownloadThread.h"

// ABPDownloadThread

IMPLEMENT_DYNCREATE(ABPDownloadThread, CWinThread)

ABPDownloadThread::ABPDownloadThread()
{
}

ABPDownloadThread::~ABPDownloadThread()
{
}

BOOL ABPDownloadThread::InitInstance()
{
	return TRUE;
}

int ABPDownloadThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(ABPDownloadThread, CWinThread)
END_MESSAGE_MAP()


void ABPDownloadThread::DownloadFeedback(DWORD totalSize, DWORD downloaded)
{
	m_totalSize = totalSize;
	m_downloaded = downloaded;
}

int ABPDownloadThread::Run(void)
{
	ABPLTools tools;
	m_path.Replace("/", "\\");
	int idx = m_path.ReverseFind('\\');
	if (idx!=-1)
		tools.CreateDirectory(m_path.Mid(0, idx));
	m_bExit = false;
	tools.DownloadFile(m_url, m_path, this, &m_bExit);
	*m_pbFinish = true;
	return 0;
}
