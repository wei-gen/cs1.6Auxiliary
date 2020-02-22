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


// 获取玩家数量
int GetPersonNumber()
{
	DWORD num = ReadMemory_DWORD(g_hProcess, g_amxmodx_mm_base + d_person_num_offset,NULL);
	return num;
}

// 获取人物坐标
POS CPerson::GetPos(int nPersonnumber)
{
	POS pos = { 0 };
	// 先读取 g_cstrike_exe_base + 0x11069BC 里面的地址addre
	// addre + 0x88 是玩家x地址，加上偏移 玩家索引号 i * 324
	// addre + 0x8c 是玩家x地址，加上偏移 玩家索引号 i * 324
	// addre + 0x90 是玩家x地址，加上偏移 玩家索引号 i * 324
	DWORD addre = ReadMemory_DWORD(g_hProcess, g_cstrike_exe_base + d_f_person, NULL);
	pos.fXpos = ReadMemory_float(g_hProcess, addre + d_res_person_x + nPersonnumber * d_persion_offset, NULL);
	pos.fYpos = ReadMemory_float(g_hProcess, addre + d_res_person_y + nPersonnumber * d_persion_offset, NULL);
	pos.fZpos = ReadMemory_float(g_hProcess, addre + d_res_person_z + nPersonnumber * d_persion_offset, NULL);
	return pos;
}

// 获取我的头部角度
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

// 求距离
float CPerson::Get_three_distance(POS another)
{
	float fDistance;
	// 求绝对值
	float DeltaX = fabs(m_pos.fXpos - another.fXpos);
	float DeltaY = fabs(m_pos.fYpos - another.fYpos);
	float DeltaZ = fabs(m_pos.fZpos - another.fZpos);
	// pow(x,y)求x^7
	// sqrtf计算平方根
	fDistance = sqrtf(pow(DeltaX, 2) + pow(DeltaY, 2) + pow(DeltaZ, 2));
	return fDistance;
}

// 是否存活
bool CPerson::IsAlive(int n_PersonNumber)
{
	// 玩家hp地址是 hl.exe + 0x11069BC = addre + 0x1E0 + index(人物索引号) * 0x324
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
	//其他的一些初始化工作
}

// 无参数表示初始化自己的数据
CPerson::CPerson()			
{
	m_heart = GetHeartAngle();
	m_pos = GetPos(0);
	m_nPersonNumber = 0;
}

// 获取偏移角度x
float CPerspective::Get_Offset_AngleX(POS enemy)
{
	float offsetX = enemy.fXpos - m_pos.fXpos;
	float offsetY = enemy.fYpos - m_pos.fYpos;
	float AngleX;

	if (offsetX == 0 || offsetY == 0)				//防止后面三角函数出现除零错误
		return 0;

	AngleX = atanf(offsetY / offsetX) * 57.29746936176986;
	if (offsetY < 0 && offsetX < 0)					//3象限，角度为正数 < 0 ~ Π/2
		AngleX = AngleX + 180;
	if (offsetY > 0 && offsetX < 0)					//第二象限，此时结果为负数0 ~ -Π/2
		AngleX = AngleX + 180;

	HEART MouseHeart = GetHeartAngle();
	AngleX = MouseHeart.HeartX - AngleX;			//敌人在视角的右边为正，敌人在视角的左边为负
	if (AngleX > 180)								//角度大于180°后比如350°的准心x和20°的人物角度差值就不正确了
		AngleX = AngleX - 360;

	return AngleX;
}

float CPerspective::Get_Offset_AngleY(POS enemy)
{
	float offsetZ = enemy.fZpos - m_pos.fZpos;
	float f2Distance = Get_two_distance(enemy);
	float AngleY;
			
	AngleY = -atanf(offsetZ / f2Distance) * 57.29746936176986;			//这里和准心Y角度方向一致
	return AngleY;
}

// 获取视场角
float CPerspective::GetFov()
{
	DWORD first = ReadMemory_DWORD(g_hProcess, g_cstrike_exe_base + d_f_person, NULL);
	float res = ReadMemory_float(g_hProcess, first + d_f_local_fov, NULL);
	return res;
}

// 重新
void CPerspective::update()
{
	m_pos = GetPos(0);
	m_heart = GetHeartAngle();
	m_fov = GetFov();
}

void CPerspective::PaintRect()
{
	// 获取玩家人数
	int nNum_Person = GetPersonNumber();
	for (int i = 1; i < nNum_Person; i++)
	{
		// 存活的就画边框
		if (IsAlive(i))
		{
			RECT rect;
			CPerson enemy(i);
			// 更新本地玩家数据
			update();
			// 获取屏幕x轴
			float AngleX = Get_Offset_AngleX(enemy.m_pos);	
			// 获取屏幕y轴
			float AngleY = Get_Offset_AngleY(enemy.m_pos);
			// 获取准心y轴
			float AngleMouse = GetHeartAngle().HeartY;			
			
			// 计算本地玩家和其他玩家的距离
			// 我的三个坐标-敌人三个坐标绝对值
			// 三个坐标的2次幂
			// 相加再计算平方根
			// 算出距离
			float f3Distance = Get_three_distance(enemy.m_pos);
			// 画的方框宽高
			int high, width;
			high = 25000 / f3Distance;
			width = 11000 / f3Distance;

			if (fabs(AngleX) > m_fov * 0.5)
				continue;
			float FovPrecent = 90 / m_fov;

			rect.left = tanf(AngleX / 57.29746936176986) * m_HalfGameWidth * FovPrecent + m_HalfGameWidth;
			// AngleMouse 鼠标角度加个负号，要取相反值
			// 不加负号会造成方框向抬起的角度上移
			rect.top = (tanf(-AngleMouse / 57.29746936176986) + tanf(AngleY / 57.29746936176986)) * m_HalfGameWidth * FovPrecent + m_HalfGameHigh;
			rect.right = rect.left + width;
			rect.bottom = rect.top + high;
			
			WCHAR strAr[20];
			CString str;
			str.Format(L"距离：%.2f\0", f3Distance);
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
