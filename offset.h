#pragma once
#pragma once
//pcawwclconfig_mm.dllģ��
#define BASE_ME			0x00097030
#define BASE_ENEMY		0x10A70
#define POS_X			0x88
#define POS_Y			0x8C
#define POS_Z			0x90
#define OFFSET_PERSON	0x30		//0x30�����ÿ���˵�ƫ��
#define TEAM			0x8			//��Ӫ
#define	HP				0x1E0		//Ѫ��

//cstrike.exe
#define HEART_X			0x19DB1C8
#define HEART_Y			0x19DB1C4
#define FOV				0x60CF24

//mp.dll
#define NUM_PERSON		0xD18EC		//��Ϸ�������

#define MAX_DISTANCE	10000


#define d_f_enemy			0x001622e4			// ��ȡ���������һ��ƫ��
#define d_res_enemy			0x58				// �ڶ���ƫ��
#define d_f_person			0x11069BC			// ��ȡ�����ַ��һ��ƫ��
#define d_res_person_x		0x88				// �ڶ���ƫ��
#define d_res_person_y		0x8C				// �ڶ���ƫ��
#define d_res_person_z		0x90				// �ڶ���ƫ��
#define d_f_local_fov       0x294				// �����ӳ��ǵڶ���ƫ��
#define d_persion_offset	0x324				// ����֮��ƫ��
#define d_local_HeartX		0x19E10c8			// ���������Ļx��
#define d_local_HeartY		d_local_HeartX-0x4	// ���������Ļy��
#define d_person_num_offset	0x96184				// �������ƫ��
// ������
//#define d_1_local_recoil	0x11069BC			//
#define d_2_local_recoil	0x7c				//
#define d_3_local_recoil	0x5d8				//
#define d_4_local_recoil	0x100				//
// ����ֵ
#define d_2_local_hp		0x7c				//
#define d_3_local_hp		0x4					//
#define d_4_local_hp		0x160				//