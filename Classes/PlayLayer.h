#pragma once

#include "cocos2d.h"
#include "Box.h"

class PlayLayer : public cocos2d::CCLayer
{
	static const int GRID_HEIGHT = 12;
	static const int GRID_WIDTH = 11;
	static const int GRID_LEFT = 2;
	static const int GRID_BUTTON = 14;
	static const int MAX_BOX_NUM = 9;
	static const int MAX_NORMALBOX_NUM = 6;
	static const float BOXMOVETIME;
	static const int BOXSIZE = 65;
	static const char* BOX_NAME[9];
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static node()" method manually
    CREATE_FUNC(PlayLayer);


	cocos2d::CCArray *m_content;
	cocos2d::CCArray *m_toRemove;
	cocos2d::CCSprite *bg;
	PlayLayer();
	~PlayLayer();
	bool checkBox();
	int repair();
	int repairSingleCol(int col);
	Box * getBoxAtPosXY(int x,int y);
	Box * boundBox;
};

