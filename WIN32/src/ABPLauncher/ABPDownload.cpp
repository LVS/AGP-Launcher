// ABPDownload.cpp : implementation file
//

#include "stdafx.h"
#include "ABPLauncher.h"
#include "ABPDownload.h"
#include "ABPDownloadThread.h"

// ABPDownload dialog

IMPLEMENT_DYNAMIC(ABPDownload, CDialog)

ABPDownload::ABPDownload(CWnd* pParent /*=NULL*/)
	: CDialog(ABPDownload::IDD, pParent)
	, m_info(_T(""))
{
}

ABPDownload::~ABPDownload()
{
}

void ABPDownload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, IDC_STATIC1, m_info);
}


BEGIN_MESSAGE_MAP(ABPDownload, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL ABPDownload::OnInitDialog()
{
	m_bThreadExited = true;
	CDialog::OnInitDialog();
	m_progress.SetRange(0, m_downloadList.size()*100);
	m_listIndex = 0;
	SetTimer(0, 10, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void ABPDownload::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);
	if (m_bThreadExited)
	{
		if (m_listIndex < (int)m_downloadList.size())
		{
			CString jar = m_downloadList[m_listIndex++];
			
			m_pThread = dynamic_cast<ABPDownloadThread *>(AfxBeginThread(RUNTIME_CLASS(ABPDownloadThread), 0, 0, CREATE_SUSPENDED));
			if (m_pThread)
			{
				m_bThreadExited = false;
				m_pThread->m_bAutoDelete = false;
				m_pThread->m_pbFinish = &m_bThreadExited;
				m_pThread->m_url = m_server + jar;
				m_pThread->m_path = m_basePath + jar;
				m_pThread->m_downloaded = 0;
				m_pThread->m_totalSize = 1;
				m_pThread->ResumeThread();
			}
		}
		else
			EndDialog(IDOK);
	}
	else if (m_pThread!=NULL)
	{
		// Display progress
		float p = ((float)m_pThread->m_downloaded*100/(float)m_pThread->m_totalSize);
		m_progress.SetPos((int)((m_listIndex-1)*100+p));
		m_info.Format("%s %.02f%%", m_pThread->m_url.Mid(m_server.GetLength()), p );
		UpdateData(FALSE);
	}
	SetTimer(0, 10, NULL);
}
