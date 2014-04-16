#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"

class SettingScene : public cocos2d::CCScene
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	cocos2d::extension::UILayer *pLayer;

	void touchMusicButton(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type);
	void touchEffectButton(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type);
	void touchClose(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type);
	void touchRestart(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type);
	void touchReturn(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type);

	void setLev(int num);
    // implement the "static node()" method manually
    CREATE_FUNC(SettingScene);
};

