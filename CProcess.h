#pragma once
#include<Windows.h>
#include<TlHelp32.h>

class CProcess
{
public:
	CProcess(DWORD dwPid);
	CProcess(const WCHAR *szProcessName);
	~CProcess();
	DWORD GetModule(const WCHAR*szModuleName);

public:
	HANDLE m_hProcess;
	DWORD m_dwPid;
};