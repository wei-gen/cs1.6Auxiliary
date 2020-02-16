#include"CProcess.h"
#include<stdio.h>


CProcess::CProcess(DWORD dwPid)
{
	HANDLE m_hProcess = OpenProcess(PROCESS_ALL_ACCESS,
		FALSE,
		dwPid);
	m_dwPid = dwPid;
}

CProcess::CProcess(const WCHAR *szProcessName)
{
	m_hProcess = NULL;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pe32 = {sizeof(pe32)};
	Process32First(hSnapshot,&pe32);
	do
	{
		if (wcscmp(pe32.szExeFile, szProcessName) == 0)
		{
			m_hProcess = OpenProcess(PROCESS_ALL_ACCESS,
				FALSE,
				pe32.th32ProcessID);
			m_dwPid = pe32.th32ProcessID;
		}
	} while (Process32Next(hSnapshot,&pe32));
}

CProcess::~CProcess()
{
	if(m_hProcess != NULL)
		CloseHandle(m_hProcess);
}

// 获取dll模块基址
DWORD CProcess::GetModule(const WCHAR* szModuleName)
{
	DWORD dwModule = 0;
	// 根据进程pid获取句柄
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwPid);
	MODULEENTRY32 module32 = {sizeof(module32)};
	// 遍历进程所有模块
	Module32First(hSnapshot,&module32);
	do
	{
		if (wcscmp(szModuleName, module32.szModule) == 0) {
			dwModule = (DWORD)module32.hModule;
			wprintf_s(TEXT("SUCESS: %s base address is %d\n"), szModuleName, dwModule);
		}
			
	}
	while (Module32Next(hSnapshot,&module32));

	if (dwModule == 0)
		wprintf(TEXT("get %s base address faild! --- error code: %d\n"), szModuleName,GetLastError());
	
	return dwModule;
}