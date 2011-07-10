#include "stdafx.h"
#pragma once

class Options
{
	static CString GPPS(LPCSTR section, LPCSTR entry, LPCSTR def, LPCSTR file);
public:
	int m_language, m_memory, m_application, m_debug;
	CString m_class, m_server;
	std::vector<CString> m_lastServer;

	void Save();
	static Options Load();
};

class InternetFeedback
{
public:
	virtual void DownloadFeedback(DWORD totalSize, DWORD downloaded)=0;
};

class ABPLTools
{
public:
	CString					GetSpecialFolder(int id=CSIDL_APPDATA);
	CString					GetWorkingFolder();
	CString					MakeCanonical(CString url);
	bool					DownloadFile(CString url, CString destination, InternetFeedback* pCIF=NULL, bool *pbExit=NULL);
	CString					DownloadString(CString url, InternetFeedback* pCIF=NULL, bool *pbExit=NULL);
	std::vector<CString>	Extract(CString &src, CString &begin, CString &end);
	bool					CreateDirectory (LPCTSTR directory);
	void					RemoveDirectory (LPCTSTR directory, bool bFileOnly, bool bDispError);
	CString					GetJavawExePath();
	bool					DownloadAndLaunch(Options options);	
};

class Property
{
public:
	CString m_name, m_value;
	Property(CString name, CString value) { m_name=name; m_value=value; }
};

class ApplicationInfo
{
public:
	CString	m_description;
	std::vector<CString> m_jars, m_extension;
	std::vector<Property> m_properties;
	CString m_mainClass;

	int LoadXML	(CString url);
};
