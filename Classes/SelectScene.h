#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

class SelectScene : public cocos2d::CCScene
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	cocos2d::extension::UILayer *pLayer;

	cocos2d::extension::UIScrollView *pScroll;

	cocos2d::extension::UIButton *getButton(int num);

	void touchButton(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type);

    // implement the "static node()" method manually
    CREATE_FUNC(SelectScene);
};

