#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "PlayLayer.h"

class GameScene : public cocos2d::CCScene
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	PlayLayer *pPlayLayer;

	cocos2d::extension::UILayer *player;

	void loadLevel(int num);
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
};

