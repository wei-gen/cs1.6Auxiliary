#include "CAim_Perspective.h"
#include "Memory.h"
#include "offset.h"
#include <locale.h>
#include <stdio.h>
#include <atlstr.h>


extern HANDLE g_hProcess;
extern DWORD  g_cstrike_exe_base;				// cstrike_exe.dll
extern DWORD  g_pcawwclconfig_mm_dll_base;		// pcawwclconfig_mm.dll
extern DWORD  g_mp_dll_base;					// mp.dll
extern DWORD  g_amxmodx_mm_base;				// amxmodx_mm.dll


// ��ȡ�������
int GetPersonNumber()
{
	DWORD num = ReadMemory_DWORD(g_hProcess, g_amxmodx_mm_base + d_person_num_offset,NULL);
	return num;
}

// ��ȡ��������
POS CPerson::GetPos(int nPersonnumber)
{
	POS pos = { 0 };
	// �ȶ�ȡ g_cstrike_exe_base + 0x11069BC ����ĵ�ַaddre
	// addre + 0x88 �����x��ַ������ƫ�� ��������� i * 324
	// addre + 0x8c �����x��ַ������ƫ�� ��������� i * 324
	// addre + 0x90 �����x��ַ������ƫ�� ��������� i * 324
	DWORD addre = ReadMemory_DWORD(g_hProcess, g_cstrike_exe_base + d_f_person, NULL);
	pos.fXpos = ReadMemory_float(g_hProcess, addre + d_res_person_x + nPersonnumber * d_persion_offset, NULL);
	pos.fYpos = ReadMemory_float(g_hProcess, addre + d_res_person_y + nPersonnumber * d_persion_offset, NULL);
	pos.fZpos = ReadMemory_float(g_hProcess, addre + d_res_person_z + nPersonnumber * d_persion_offset, NULL);
	return pos;
}

// ��ȡ�ҵ�ͷ���Ƕ�
HEART CPerson::GetHeartAngle()
{
	HEART heart = { 0 };
	heart.HeartX = ReadMemory_float(g_hProcess, g_cstrike_exe_base + d_local_HeartX, NULL);
	heart.HeartY = ReadMemory_float(g_hProcess, g_cstrike_exe_base + d_local_HeartY, NULL);
	return heart;
}

float CPerson::Get_two_distance(POS another)
{
	float fDistance;
	float DeltaX = fabs(m_pos.fXpos - another.fXpos);
	float DeltaY = fabs(m_pos.fYpos - another.fYpos);
	fDistance = sqrtf(pow(DeltaX,2) + pow(DeltaY,2));
	return fDistance;
}

float CPerson::Get_two_distance(POS one, POS two)
{
	float fDistance;
	float DeltaX = fabs(one.fXpos - two.fXpos);
	float DeltaY = fabs(one.fYpos - two.fYpos);
	fDistance = sqrtf(pow(DeltaX, 2) + pow(DeltaY, 2));
	return fDistance;
}

// �����
float CPerson::Get_three_distance(POS another)
{
	float fDistance;
	// �����ֵ
	float DeltaX = fabs(m_pos.fXpos - another.fXpos);
	float DeltaY = fabs(m_pos.fYpos - another.fYpos);
	float DeltaZ = fabs(m_pos.fZpos - another.fZpos);
	// pow(x,y)��x^7
	// sqrtf����ƽ����
	fDistance = sqrtf(pow(DeltaX, 2) + pow(DeltaY, 2) + pow(DeltaZ, 2));
	return fDistance;
}

// �Ƿ���
bool CPerson::IsAlive(int n_PersonNumber)
{
	// ���hp��ַ�� hl.exe + 0x11069BC = addre + 0x1E0 + index(����������) * 0x324
	DWORD hp_addre = ReadMemory_DWORD(g_hProcess,g_cstrike_exe_base + 0x11069BC,NULL);
	float hp = ReadMemory_DWORD(g_hProcess, hp_addre + 0x1E0 + n_PersonNumber * 0x324,NULL);
	return hp > 0;
}

void CPerson::update()
{
	m_pos = GetPos(0);
	m_heart = GetHeartAngle();
}

CPerson::CPerson(int nPersonNumber)
{
	m_nPersonNumber = nPersonNumber;
	m_pos = GetPos(m_nPersonNumber);
	//������һЩ��ʼ������
}

// �޲�����ʾ��ʼ���Լ�������
CPerson::CPerson()			
{
	m_heart = GetHeartAngle();
	m_pos = GetPos(0);
	m_nPersonNumber = 0;
}

// ��ȡƫ�ƽǶ�x
float CPerspective::Get_Offset_AngleX(POS enemy)
{
	float offsetX = enemy.fXpos - m_pos.fXpos;
	float offsetY = enemy.fYpos - m_pos.fYpos;
	float AngleX;

	if (offsetX == 0 || offsetY == 0)				//��ֹ�������Ǻ������ֳ������
		return 0;

	AngleX = atanf(offsetY / offsetX) * 57.29746936176986;
	if (offsetY < 0 && offsetX < 0)					//3���ޣ��Ƕ�Ϊ���� < 0 ~ ��/2
		AngleX = AngleX + 180;
	if (offsetY > 0 && offsetX < 0)					//�ڶ����ޣ���ʱ���Ϊ����0 ~ -��/2
		AngleX = AngleX + 180;

	HEART MouseHeart = GetHeartAngle();
	AngleX = MouseHeart.HeartX - AngleX;			//�������ӽǵ��ұ�Ϊ�����������ӽǵ����Ϊ��
	if (AngleX > 180)								//�Ƕȴ���180������350���׼��x��20�������ǶȲ�ֵ�Ͳ���ȷ��
		AngleX = AngleX - 360;

	return AngleX;
}

float CPerspective::Get_Offset_AngleY(POS enemy)
{
	float offsetZ = enemy.fZpos - m_pos.fZpos;
	float f2Distance = Get_two_distance(enemy);
	float AngleY;
			
	AngleY = -atanf(offsetZ / f2Distance) * 57.29746936176986;			//�����׼��Y�Ƕȷ���һ��
	return AngleY;
}

// ��ȡ�ӳ���
float CPerspective::GetFov()
{
	DWORD first = ReadMemory_DWORD(g_hProcess, g_cstrike_exe_base + d_f_person, NULL);
	float res = ReadMemory_float(g_hProcess, first + d_f_local_fov, NULL);
	return res;
}

// ����
void CPerspective::update()
{
	m_pos = GetPos(0);
	m_heart = GetHeartAngle();
	m_fov = GetFov();
}

void CPerspective::PaintRect()
{
	// ��ȡ�������
	int nNum_Person = GetPersonNumber();
	for (int i = 1; i < nNum_Person; i++)
	{
		// ���ľͻ��߿�
		if (IsAlive(i))
		{
			RECT rect;
			CPerson enemy(i);
			// ���±����������
			update();
			// ��ȡ��Ļx��
			float AngleX = Get_Offset_AngleX(enemy.m_pos);	
			// ��ȡ��Ļy��
			float AngleY = Get_Offset_AngleY(enemy.m_pos);
			// ��ȡ׼��y��
			float AngleMouse = GetHeartAngle().HeartY;			
			
			// ���㱾����Һ�������ҵľ���
			// �ҵ���������-���������������ֵ
			// ���������2����
			// ����ټ���ƽ����
			// �������
			float f3Distance = Get_three_distance(enemy.m_pos);
			// ���ķ�����
			int high, width;
			high = 25000 / f3Distance;
			width = 11000 / f3Distance;

			if (fabs(AngleX) > m_fov * 0.5)
				continue;
			float FovPrecent = 90 / m_fov;

			rect.left = tanf(AngleX / 57.29746936176986) * m_HalfGameWidth * FovPrecent + m_HalfGameWidth;
			// AngleMouse ���ǶȼӸ����ţ�Ҫȡ�෴ֵ
			// ���Ӹ��Ż���ɷ�����̧��ĽǶ�����
			rect.top = (tanf(-AngleMouse / 57.29746936176986) + tanf(AngleY / 57.29746936176986)) * m_HalfGameWidth * FovPrecent + m_HalfGameHigh;
			rect.right = rect.left + width;
			rect.bottom = rect.top + high;
			
			WCHAR strAr[20];
			CString str;
			str.Format(L"���룺%.2f\0", f3Distance);
			wsprintf(strAr,L"%s", str.GetBuffer(str.GetLength()));
			
			TextOut(m_hdc, rect.left, rect.top, strAr, 30);
			Rectangle(m_hdc, rect.left, rect.top, rect.right, rect.bottom);
		}
	}

	
}

void CPerspective::StartPaint()
{
	
	m_PaintStatus = true;
	while (m_PaintStatus)
	{
		PaintRect();
		//Sleep(200);
		if (!m_PaintStatus)
			break;
	}
}

void CPerspective::StopPaint()
{
	m_PaintStatus = false;
}
