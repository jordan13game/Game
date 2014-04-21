#include "GameScene.h"
#include "SettingScene.h"


USING_NS_CC;
USING_NS_CC_EXT;

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCScene::init() )
    {
        return false;
    }
    
	srand(time(NULL));

    pPlayLayer = PlayLayer::create();

	this->addChild(pPlayLayer);
    this->setTag(30);

    return true;
}

void GameScene::loadLevel( int num )
{
	level = num;
	player = UILayer::create();
	player->addWidget(CCUIHELPER->createWidgetFromJsonFile("head/Head.ExportJson"));
	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	this->addChild(player,1,1);
	CCSize tmp=player->getWidgetByName("Panel")->getSize();
	player->setPosition(0,sz.height-tmp.height);
	pPlayLayer->startGame(num);
	UIButton *bt = (UIButton *)player->getWidgetByName("Panel")->getChildByName("setting");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(GameScene::touchButton));
}

void GameScene::touchButton( CCObject* obj,TouchEventType type )
{
	if (type == TOUCH_EVENT_ENDED)
	{
		keyBackClicked();
	}
	
}

void GameScene::keyBackClicked()
{
	SettingScene *scene = SettingScene::create();
	scene->setLev(level);
	CCDirector::sharedDirector()->pushScene(scene);
}
