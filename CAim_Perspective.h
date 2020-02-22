#pragma once
#include<Windows.h>
#include<math.h>

int GetPersonNumber();

//准心结构
typedef struct HEART		
{
	float HeartX;
	float HeartY;
}HEART;

//三维坐标结构
typedef struct POS
{
	float fXpos;
	float fYpos;
	float fZpos;
}POS;


class CPerson
{
public:
	HEART m_heart;
	POS	  m_pos;
	int	  m_nPersonNumber;
	int	  m_nTeam;
	static POS GetPos(int nPersonnumber);
	static HEART GetHeartAngle();
	float Get_two_distance(POS another);
	float Get_two_distance(POS one,POS two);
	float Get_three_distance(POS another);
	//int GetTeam(int nPersonnum);
	//int GetTeam();
	bool IsAlive(int n_PersonNumber);
	virtual void update();
	CPerson(int nPersonNumber);
	CPerson();
};

// 画图框类
class CPerspective : public CPerson
{
private:
	float m_fov;
	RECT  m_GameRect;
	int   m_HalfGameWidth;
	int   m_HalfGameHigh;
	bool  m_PaintStatus;
	HDC	  m_hdc;
	HWND  m_hwnd;
	float Get_Offset_AngleX(POS enemy);
	float Get_Offset_AngleY(POS enemy);
	float GetFov();
	void PaintRect();
	virtual void update();

public:
	CPerspective(const wchar_t*szWindowName,const wchar_t *szClssName = NULL) :CPerson() {
		m_fov = GetFov();
		m_PaintStatus = false;
		m_hwnd = FindWindow(szClssName,szWindowName);
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		m_hdc = GetDC(m_hwnd);
		SelectObject(m_hdc, hPen);
		SelectObject(m_hdc, hBrush);
		// 获取屏幕客户端界面
		GetClientRect(m_hwnd,&m_GameRect);
		m_HalfGameWidth = (m_GameRect.right - m_GameRect.left) * 0.5;
		m_HalfGameHigh = (m_GameRect.bottom - m_GameRect.top) * 0.5;
	}
	~CPerspective() {
		ReleaseDC(m_hwnd,m_hdc);
	}
	void StartPaint();
	void StopPaint();
};