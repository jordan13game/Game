#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

class StartScene : public cocos2d::CCScene
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	cocos2d::extension::UILayer *pLayer;

	void touchStart(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type);
    // implement the "static node()" method manually
    CREATE_FUNC(StartScene);

	void initUserInfo();

	virtual void keyBackClicked();
};

