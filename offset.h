#pragma once
#pragma once
//pcawwclconfig_mm.dll模块
#define BASE_ME			0x00097030
#define BASE_ENEMY		0x10A70
#define POS_X			0x88
#define POS_Y			0x8C
#define POS_Z			0x90
#define OFFSET_PERSON	0x30		//0x30间隔，每个人的偏移
#define TEAM			0x8			//阵营
#define	HP				0x1E0		//血量

//cstrike.exe
#define HEART_X			0x19DB1C8
#define HEART_Y			0x19DB1C4
#define FOV				0x60CF24

//mp.dll
#define NUM_PERSON		0xD18EC		//游戏玩家数量

#define MAX_DISTANCE	10000


#define d_f_enemy			0x001622e4			// 获取玩家数量第一个偏移
#define d_res_enemy			0x58				// 第二个偏移
#define d_f_person			0x11069BC			// 获取人物地址第一个偏移
#define d_res_person_x		0x88				// 第二个偏移
#define d_res_person_y		0x8C				// 第二个偏移
#define d_res_person_z		0x90				// 第二个偏移
#define d_f_local_fov       0x294				// 人物视场角第二个偏移
#define d_persion_offset	0x324				// 人物之间偏移
#define d_local_HeartX		0x19E10c8			// 本地玩家屏幕x轴
#define d_local_HeartY		d_local_HeartX-0x4	// 本地玩家屏幕y轴
#define d_person_num_offset	0x96184				// 玩家数量偏移
// 后座力
//#define d_1_local_recoil	0x11069BC			//
#define d_2_local_recoil	0x7c				//
#define d_3_local_recoil	0x5d8				//
#define d_4_local_recoil	0x100				//
// 生命值
#define d_2_local_hp		0x7c				//
#define d_3_local_hp		0x4					//
#define d_4_local_hp		0x160				//