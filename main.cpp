#include"CAim_Perspective.h"
#include"CProcess.h"
#include"Memory.h"
#include"offset.h"
#include<stdio.h>

HANDLE g_hProcess;
DWORD  g_cstrike_exe_base;
DWORD  g_pcawwclconfig_mm_dll_base;
DWORD  g_mp_dll_base;
DWORD  g_amxmodx_mm_base;
CProcess Cproc(TEXT("hl.exe"));

void init();

// 无后座
void No_recoil()
{
	CreateThread(NULL,
		0,
		[](LPVOID lpThreadParameter)->DWORD {
		while (TRUE)
		{
			DWORD first = ReadMemory_DWORD(Cproc.m_hProcess, g_cstrike_exe_base + d_f_person, NULL);
			DWORD second = ReadMemory_DWORD(Cproc.m_hProcess, first + d_2_local_recoil, NULL);
			DWORD third = ReadMemory_DWORD(Cproc.m_hProcess, second + d_3_local_recoil, NULL);
			DWORD ret = WriteMemory_DWORD(Cproc.m_hProcess, third + d_4_local_recoil, 0);
		}
	},
		NULL,
		0,
		NULL
		);
}

// 无限生命
void ManyHp()	
{
	CreateThread(NULL,
		0,
		[](LPVOID lpThreadParameter)->DWORD {
		while (TRUE)
		{
			float fHp = 200.0;
			DWORD first = ReadMemory_DWORD(Cproc.m_hProcess, g_cstrike_exe_base + d_f_person, NULL);
			DWORD second = ReadMemory_DWORD(Cproc.m_hProcess, first + d_2_local_hp, NULL);
			DWORD third = ReadMemory_DWORD(Cproc.m_hProcess, second + d_3_local_hp, NULL);
			// &fHp--》地址
			// (DWORD*)&fHp--》变成一个指针
			// *(DWORD*)&fHp--》 变成int类型的值，因为float和int都是4个字节，但是保存方式不一样
			// 导致int显示的值特别大
			DWORD ret = WriteMemory_DWORD(Cproc.m_hProcess, third + d_4_local_hp, *(DWORD*)&fHp);
			Sleep(5);
		}
	},
		NULL,
		0,
		NULL
		);
}

// 无限金钱
void ManyMoney()
{
	CreateThread(NULL,
		0,
		[](LPVOID lpThreadParameter)->DWORD {
			while (TRUE)
			{
				DWORD first = ReadMemory_DWORD(Cproc.m_hProcess, g_cstrike_exe_base + 0x011069bc, NULL);
				DWORD second = ReadMemory_DWORD(Cproc.m_hProcess, first + 0x8, NULL);
				DWORD third = ReadMemory_DWORD(Cproc.m_hProcess, second + 0x4, NULL);
				DWORD fourth = ReadMemory_DWORD(Cproc.m_hProcess, third + 0x74, NULL);
				//printf("%x",)
				DWORD ret = WriteMemory_DWORD(Cproc.m_hProcess, fourth + 0x1cc, 8000);
				Sleep(10);
			}
		},
		NULL,
			0,
			NULL
			);
}

// 无限子弹
void ManyBullet()	
{
	CreateThread(NULL,
		0,
		[](LPVOID lpThreadParameter)->DWORD {
		while (TRUE)
		{
			DWORD first = ReadMemory_DWORD(Cproc.m_hProcess, g_amxmodx_mm_base + 0x00097030, NULL);
			DWORD second = ReadMemory_DWORD(Cproc.m_hProcess, first + 0x7C, NULL);
			DWORD third = ReadMemory_DWORD(Cproc.m_hProcess, second + 0x5c8, NULL);
			DWORD four = ReadMemory_DWORD(Cproc.m_hProcess, third + 0xa4, NULL);
			DWORD fifth = ReadMemory_DWORD(Cproc.m_hProcess, four + 0x5d4, NULL);
			DWORD ret = WriteMemory_DWORD(Cproc.m_hProcess, fifth + 0xcc, 20);
			Sleep(10);
		}
	},
		NULL,
		0,
		NULL
		);
}

DWORD ThreadPaint(LPVOID)
{
	CPerspective Paint(TEXT("Counter-Strike"), TEXT("Valve001"));
	Paint.StartPaint();
	return 0;
}



void main()
{
	init();
	//ManyBullet();
	//No_recoil();
	//ManyHp();
	//ManyMoney();
	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadPaint,NULL,0,0);
	//CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadAimBot,NULL,0,0);
	getchar();
}

// 获取进程中某些模块的基址
void init()
{
	g_pcawwclconfig_mm_dll_base = Cproc.GetModule(TEXT("pcawwclconfig_mm.dll"));
	g_cstrike_exe_base = Cproc.GetModule(TEXT("hl.exe"));
	g_hProcess = Cproc.m_hProcess;
	g_mp_dll_base = Cproc.GetModule(TEXT("mp.dll"));
	g_amxmodx_mm_base = Cproc.GetModule(TEXT("amxmodx_mm.dll"));
}