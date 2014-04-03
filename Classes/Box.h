#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class Box : public cocos2d::CCNode
{
	static const int COMBO_TYPE_NONE	= 0;
	static const int COMBO_TYPE_ROW		= 1;
	static const int COMBO_TYPE_COL		= 2;
protected:
	
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	int combo_type;//1代表横着 2代表竖着
	CCSprite *pSprite;
	int _type;
	int x , y;
    // implement the "static node()" method manually
    CREATE_FUNC(Box);
};

