#include "stdafx.h"
#include "ABPLTools.h"
#include "ABPDownload.h"

CString ABPLTools::GetSpecialFolder(int id)
{
	CString result;
	LPMALLOC        pMalloc;
    // Retrieve the task memory allocator.
    if (SUCCEEDED(SHGetMalloc(&pMalloc)))
    {
        ITEMIDLIST *    list;
        // Get name as PIDL
        if(!(SHGetSpecialFolderLocation(NULL, id, &list)))
        {
            TCHAR    SDestPath[MAX_PATH];
            // Convert PIDL to string
            SHGetPathFromIDList(list, (LPTSTR)&SDestPath[0]);
            // Free the PIDL allocated by SHGetSpecialFolderLocation
            pMalloc->Free((void *)list);
			result = SDestPath;
        }
        // Release the shell's allocator 
        pMalloc->Release();
	}
	return result;
}

CString	ABPLTools::GetWorkingFolder()
{
	CString appLocal = GetSpecialFolder() + TEXT("\\ABP Launcher\\");
	CreateDirectory(appLocal);
	return appLocal;
}

CString ABPLTools::MakeCanonical(CString url)
{
	// http://live.lvs.co.uk:8080/agp/apps/trayItem.jnlp

	if (url.Find(TEXT("http://"))==-1)
		url = TEXT("http://")+url;
	if (url.Find(TEXT(":8080"))==-1)
		url = url + TEXT(":8080");
	if (url.Find(TEXT("/abp/"))==-1)
		url = url + TEXT("/abp/");
	
	return url;
}

bool ABPLTools::DownloadFile(CString url, CString destination, InternetFeedback* pCIF, bool *pbExit)
{
bool			bResult = false ;
HINTERNET		hSession, hConnect ;
char			szTemp[512] ;
DWORD			dwBytesRead ;

	if (pbExit==NULL)
		pbExit = &bResult;

	if (hSession = InternetOpen (TEXT("ABP-Launcher (Win)"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0))
	{
		/*unsigned long to = Timeout;
		InternetSetOption(hSession, INTERNET_OPTION_CONNECT_TIMEOUT, &to, sizeof(unsigned long));*/
		if (hConnect = InternetOpenUrl (hSession, url, NULL, 0, INTERNET_FLAG_RELOAD, 0))
		{
			DWORD dwSize = 512, dwIndex=0;
			HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH, szTemp, &dwSize, &dwIndex);
			DWORD totalSize = atol(szTemp);
			HANDLE hFile = CreateFile(destination, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, 0);
			if (hFile!=INVALID_HANDLE_VALUE)
			{
				DWORD dwTotal=0;
				do
				{	
					dwBytesRead=0;
					if (InternetReadFile (hConnect, szTemp, sizeof (szTemp)-1, &dwBytesRead))
					{
						DWORD dwWritten;
						WriteFile(hFile, szTemp, dwBytesRead, &dwWritten, NULL);
						dwTotal += dwWritten;
						if (pCIF)
							pCIF->DownloadFeedback(totalSize, dwTotal);
					}
				}
				while (dwBytesRead > 0 && *pbExit==false);
				bResult = !*pbExit ;
				CloseHandle(hFile);
				if (!bResult)
					DeleteFile(destination);
			}
			InternetCloseHandle (hConnect) ;
		}
		InternetCloseHandle (hSession) ;
	}
	return bResult ;
}

CString	ABPLTools::DownloadString(CString url, InternetFeedback* pCIF, bool *pbExit)
{
CString			result;
HINTERNET		hSession, hConnect ;
char			szTemp[512] ;
DWORD			dwBytesRead ;
bool			bExit = false;

	if (pbExit==NULL)
		pbExit = &bExit;

	if (hSession = InternetOpen (TEXT("ABP-Launcher (Win)"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0))
	{
		/*unsigned long to = Timeout;
		InternetSetOption(hSession, INTERNET_OPTION_CONNECT_TIMEOUT, &to, sizeof(unsigned long));*/
		if (hConnect = InternetOpenUrl (hSession, url, NULL, 0, INTERNET_FLAG_RELOAD, 0))
		{
			DWORD dwSize = 512, dwIndex=0;
			HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH, szTemp, &dwSize, &dwIndex);
			DWORD totalSize = atol(szTemp);
			DWORD dwTotal=0;
			do
			{	
				dwBytesRead=0;
				if (InternetReadFile (hConnect, szTemp, sizeof (szTemp)-1, &dwBytesRead))
				{
					szTemp[dwBytesRead]=0;
					result.Append(szTemp);
					dwTotal += dwBytesRead;
					if (pCIF)
						pCIF->DownloadFeedback(totalSize, dwTotal);
				}
			}
			while (dwBytesRead > 0 && *pbExit==false);
			InternetCloseHandle (hConnect) ;
		}
		InternetCloseHandle (hSession) ;
	}
	return result ;
}

std::vector<CString> ABPLTools::Extract(CString &src, CString &begin, CString &end)
{
	std::vector<CString> result;
	int idxBegin, idxEnd, idx = 0;
	do
	{
		idxBegin = src.Find(begin, idx);
		if (idxBegin!=-1)
		{
			int idxBegin2 = idxBegin+begin.GetLength();
			idxEnd = src.Find(end, idxBegin2);
			if (idxEnd!=-1)
			{
				result.push_back(src.Mid(idxBegin2, idxEnd-idxBegin2));
				idx = idxEnd + end.GetLength();
			}
		}
	}
	while (idxBegin!=-1 && idxEnd!=-1);
	return result;
}

bool ABPLTools::CreateDirectory (LPCTSTR directory)
{
	bool bResult=true;
	TCHAR tmp[1024];
	lstrcpy (tmp, directory);
	TCHAR *Pt = tmp;
	
	if (_tcsncmp(Pt, TEXT("\\\\"), 2)==0) // Cas chemin UNC
	{
		Pt += 2;
		while (*Pt && *Pt!=TEXT('\\'))
			Pt++;
		if (*Pt==TEXT('\\'))
			Pt++;
	}

	while (*Pt && bResult)
	{
		// Find the next directory
		while (*Pt && *Pt!=TEXT('\\'))
			Pt++;
		if (*Pt==TEXT('\\'))
		{
			DWORD le = 0;
			*Pt=0;
			DWORD attrib = GetFileAttributes(tmp);
			if ((attrib & FILE_ATTRIBUTE_DIRECTORY)==0 || attrib==INVALID_FILE_ATTRIBUTES)
			{
				bResult = ::CreateDirectory(tmp, NULL)?true:false;
				le = GetLastError();
			}
			*Pt=TEXT('\\');
			Pt++;
		}
	}

	if (bResult)
	{
		DWORD attrib = GetFileAttributes(tmp);
		if ((attrib & FILE_ATTRIBUTE_DIRECTORY)==0 || attrib==INVALID_FILE_ATTRIBUTES)
			bResult = ::CreateDirectory(directory, NULL)?true:false;
	}

	return bResult;
}

void ABPLTools::RemoveDirectory (LPCTSTR directory, bool bFileOnly, bool bDispError)
{
	WIN32_FIND_DATA	findData;

	if (lstrlen(directory)>0)
	{
		TCHAR buffer[512];
		CString search;
		search.Format(TEXT("%s\\*"), directory);
		// Find sub directories to remove them
		HANDLE hFind = FindFirstFile(search, &findData);
		if (hFind!=INVALID_HANDLE_VALUE)
		{
			do
			{
				CString src;
				src.Format(TEXT("%s\\%s"), directory, findData.cFileName);

				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (_tcscmp(findData.cFileName, TEXT(".")) && _tcscmp(findData.cFileName, TEXT("..")))
						RemoveDirectory(src, bFileOnly, bDispError);
				}
				else
				{
					SetFileAttributes(src, FILE_ATTRIBUTE_NORMAL);
					if (!DeleteFile(src) && bDispError)
					{
						DWORD dwError = GetLastError();
						wsprintf(buffer, TEXT("DeleteFile(%s)=0x%x"), (LPCSTR)src, dwError);
						MessageBox(NULL, buffer, TEXT("RemoveDirectory"), MB_OK);
					}
				}
			}
			while (FindNextFile(hFind, &findData));
			FindClose(hFind);
		}

		if (!bFileOnly)
		{
			DWORD attrib = GetFileAttributes(directory);
			if (attrib!=INVALID_FILE_ATTRIBUTES) // The file doesn't exist
			{
				attrib &= ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributes(directory, attrib);
				if (!::RemoveDirectory(directory) && bDispError)
				{
					DWORD dwError = GetLastError();
					wsprintf(buffer, TEXT("RemoveDirectory(%s)=0x%x"), directory, dwError);
					MessageBox(NULL, buffer, TEXT("RemoveDirectory"), MB_OK);
				}
			}
		}
	}
}

CString	ABPLTools::GetJavawExePath()
{
	CString result;
	CRegKey regKey;
	if (regKey.Open(HKEY_CLASSES_ROOT, "jarfile\\shell\\open\\command", KEY_READ)==ERROR_SUCCESS)
	{
		char buffer[4096];
		ULONG size = sizeof(buffer);
		if (regKey.QueryStringValue("", buffer, &size)==ERROR_SUCCESS)
		{
			// We get "C:\Program Files (x86)\Java\jre6\bin\javaw.exe" -jar "%1" %*
			result = buffer+1;
			result = result.Mid(0, result.Find('"'));
		}
	}
	return result;
}

bool ABPLTools::DownloadAndLaunch(Options options)
{
	bool bResult=false;
	CString appLocal = GetWorkingFolder();
	CString server = MakeCanonical(options.m_server);	// Build URL
	// Find application
	CString xmlName[] = { "trayItem.jnlp", "tms_local.jnlp", "retailMonitor.jnlp", "trniviewer.jnlp" };
	ApplicationInfo application;
	CString appName = xmlName[options.m_application];
	application.LoadXML(server + "apps/" + appName);
	//int idx = appName.ReverseFind('/');
	//CString file = appName.Mid(idx+1);
	//tools.DownloadFile(server + "apps/" + appName, appLocal + file);

	std::vector<CString> resources, resources32, resources64, resourcesMac;
	for (int i=0; i<(int)application.m_extension.size(); i++)
	{
		CString thirdParty = DownloadString(server + application.m_extension[i]);
		// tools.DownloadFile(server + application.m_extension[i], appLocal + "thirdParty.xml");
		TiXmlElement* element;
		TiXmlDocument doc1;
		doc1.Parse(thirdParty);
		TiXmlHandle handle0(&doc1);
		int idx=0;
		do
		{
			TiXmlHandle handle1 = handle0.FirstChild("jnlp").Child("resources", idx++);
			element = handle1.ToElement();
			if (element)
			{
				std::vector<CString> *pVector = &resources;
				const char *os = element->Attribute("os");
				const char *arch = element->Attribute("arch");
				const char *family = element->Attribute("family");
				
				if (os)
				{
					if (!strcmp(os, "Windows"))
					{
						if (arch)
						{
							if (!strcmp(arch, "x86"))
								pVector = &resources32;
							else if (!strcmp(arch, "x86_64"))
								pVector = &resources64;
						}
					}

					if (strstr(os, "Mac"))
					{
						pVector = &resourcesMac;
					}

				}
			
				if (!family)
				{
					int idx2=0;
					TiXmlElement* element2;
					do
					{
						element2 = handle1.Child("jar", idx2++).ToElement();
						if (element2)
							pVector->push_back(element2->Attribute("href"));
					}
					while (element2);
				}

			}
		}  while (element);
	}

	std::vector<CString> downloadList;
	std::copy(resources.begin(), resources.end(), std::back_inserter(downloadList));
	std::copy(resources32.begin(), resources32.end(), std::back_inserter(downloadList));
	std::copy(application.m_jars.begin(), application.m_jars.end(), std::back_inserter(downloadList));
	CString fileServer = server;
	fileServer.Replace("/", "_");
	fileServer.Replace(":", "_");
	fileServer.Append("\\");

	ABPDownload download;
	download.m_server = server;
	download.m_basePath = appLocal + fileServer;

	// Download only missing file
	for (std::vector<CString>::iterator it = downloadList.begin(); it != downloadList.end(); it++)
	{
		CString final = download.m_basePath + *it;
		final.Replace("/", "\\");
		DWORD exist = GetFileAttributes(final); // Easy way to see if a file exists
		if (exist==INVALID_FILE_ATTRIBUTES)
			download.m_downloadList.push_back(*it);
	}

	INT_PTR result = IDOK;
	if (download.m_downloadList.size()>0)
		result = download.DoModal();
	if (result==IDOK)
	{
		// Create argument list
		CString javaw = GetJavawExePath();
		CString CmdLine;
		int memory[] = { 256, 512, 1024 };
		CmdLine.Format("-Xmx%dm ", memory[options.m_memory]);
		for (int i=0; i<(int)application.m_properties.size(); i++)
		{
			CmdLine.Append("-D");
			CmdLine.Append(application.m_properties[i].m_name);
			CmdLine.Append("=");
			CmdLine.Append(application.m_properties[i].m_value);
			CmdLine.Append(" ");
		}

		CString lang[] = { "en_GB", "fr_FR", "iw_IL" };
		CmdLine.Append("-Dlocale=");
		CmdLine.Append(lang[options.m_language]);
		CmdLine.Append(" ");

		if (options.m_debug)
			CmdLine.Append("-Ddebug=true ");

		CmdLine.Append("-classpath ");
		CString sep = ";";
		CmdLine.Append("\"");
		CmdLine.Append(download.m_basePath);
		CmdLine.Append("webstart-apps\\lib\\*\"");
		
		CmdLine.Append(" ");
		if (options.m_class.GetLength()>0)
			CmdLine.Append(options.m_class);
		else
			CmdLine.Append(application.m_mainClass);

		STARTUPINFO	StartupInfo = {};
		PROCESS_INFORMATION ProcessInformation = {};
		CreateProcess (javaw, (LPSTR)(LPCSTR)CmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &StartupInfo, &ProcessInformation);
		CFile file(appLocal + "log.txt", CFile::modeCreate | CFile::modeReadWrite);
		CString out = "\"" + javaw + "\" " + CmdLine;
		file.Write((LPCSTR)out, out.GetLength());
		file.Close();
		bResult=true;
	}
	return bResult;
}


int ApplicationInfo::LoadXML(CString url)
{
	ABPLTools tools;
	CString xml = tools.DownloadString(url);
	if (xml.GetLength()>0)
	{
		TiXmlDocument doc1;
		doc1.Parse(xml);
		TiXmlHandle handle0(&doc1);
		m_description = handle0.FirstChild("jnlp").FirstChild("information").FirstChild("description").ToElement()->GetText();
		m_mainClass = handle0.FirstChild("jnlp").FirstChild("application-desc").ToElement()->Attribute("main-class");
		TiXmlElement* element, *root;
		for (int j=0; (root = handle0.FirstChild("jnlp").Child("resources", j).ToElement())!=NULL; j++)
		{
			TiXmlHandle root0(root);
			for (int i=0; (element = root0.Child("jar", i).ToElement())!=NULL; i++)
				m_jars.push_back(element->Attribute("href"));
			for (int i=0; (element = root0.Child("nativelib", i).ToElement())!=NULL; i++)
				m_jars.push_back(element->Attribute("href"));
			for (int i=0; (element = root0.Child("extension", i).ToElement())!=NULL; i++)
				m_extension.push_back(element->Attribute("href"));
			for (int i=0; (element = root0.Child("property", i).ToElement())!=NULL; i++)
			{
				Property p(element->Attribute("name"), element->Attribute("value"));
				m_properties.push_back(p);
			}
		}
	}
	return m_jars.size();
}

CString Options::GPPS(LPCSTR section, LPCSTR entry, LPCSTR def, LPCSTR file)
{
	char tmp[512];
	::GetPrivateProfileString(section, entry, def, tmp, sizeof(tmp), file);
	return CString(tmp);
}


Options Options::Load()
{
	Options result;
	ABPLTools tools;
	CString appLocal = tools.GetWorkingFolder() + "ABPLauncher.ini";
	char *app = "ABPLauncher";
	result.m_language = ::GetPrivateProfileInt(app, "m_language", 0, appLocal);
	result.m_memory = ::GetPrivateProfileInt(app, "m_memory", 1, appLocal);
	result.m_application = ::GetPrivateProfileInt(app, "m_application", 0, appLocal);
	result.m_debug = ::GetPrivateProfileInt(app, "m_debug", 0, appLocal);
	result.m_class = GPPS(app, "m_class", "", appLocal);
	result.m_server = GPPS(app, "m_server", "live.lvs.co.uk", appLocal);
	int idx=0;
	CString tmp2;
	do
	{
		char tmp[512];
		wsprintf(tmp, "m_lastServer%d", idx++);
		tmp2 = GPPS(app, tmp, "", appLocal);
		if (tmp2.GetLength()>0)
			result.m_lastServer.push_back(tmp2);
	}
	while (tmp2.GetLength()>0);
	if (result.m_lastServer.size()==0)
		result.m_lastServer.push_back(result.m_server);
	return result;
}

void Options::Save()
{
	ABPLTools tools;
	CString appLocal = tools.GetWorkingFolder() + "ABPLauncher.ini";
	::DeleteFile(appLocal);
	char *app = "ABPLauncher";
	char tmp[512];
	_itoa_s(m_language, tmp, sizeof(tmp), 10);
	::WritePrivateProfileString(app, "m_language", tmp, appLocal);
	_itoa_s(m_memory, tmp, sizeof(tmp), 10);
	::WritePrivateProfileString(app, "m_memory", tmp, appLocal);
	_itoa_s(m_application, tmp, sizeof(tmp), 10);
	::WritePrivateProfileString(app, "m_application", tmp, appLocal);
	_itoa_s(m_debug, tmp, sizeof(tmp), 10);
	::WritePrivateProfileString(app, "m_debug", tmp, appLocal);
	::WritePrivateProfileString(app, "m_class", m_class, appLocal);
	::WritePrivateProfileString(app, "m_server", m_server, appLocal);
	int idx=0;
	for (std::vector<CString>::iterator it = m_lastServer.begin(); it != m_lastServer.end(); it++)
	{
		wsprintf(tmp, "m_lastServer%d", idx++);
		::WritePrivateProfileString(app, tmp, *it, appLocal);
	}

}