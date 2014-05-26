#pragma once

//连击类型
static const int COMBO_TYPE_NONE	= 0;
static const int COMBO_TYPE_ROW		= 1;
static const int COMBO_TYPE_COL		= 2;
static const int COMBO_TYPE_ALL		= 3;
static const int COMBO_TYPE_SQR		= 4;
//一些常数
static const int GRID_HEIGHT = 8;
static const int GRID_WIDTH = 8;
static const int GRID_LEFT = 20;
static const int GRID_BUTTON = 50;
static const int MAX_BOX_NUM = 9;
static const int MAX_NORMALBOX_NUM = 8;
static const float BOXMOVETIME = 0.05f;
static const float BOXSWAPTIME = 0.2f;
static const float BOXDISTIME = 0.3f;
static const int BOXSIZE = 85;
static const char* BOX_NAME[9] = {"bubble_0.png","bubble_1.png","bubble_2.png","bubble_3.png","bubble_4.png"
	,"bubble_5.png","bubble_6.png","bubble_7.png","bubble_8.png"};
//检测小球横竖
static const int HORIZON = 0;
static const int VERTICAL = 1;

//游戏类型
static const int GAMETYPE_TIME = 0;
static const int GAMETYPE_STEP = 1;
//一个球得分
static const int SCORE = 50;
//延迟
static const float HINT_DELAY = 5.0f;
static const float SCORE_DELAY = 1.5f;

//关卡数
static const int TOTLEVEL = 20;