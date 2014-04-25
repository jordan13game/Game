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
	
	UIButton *bt = (UIButton *)player->getWidgetByName("Panel")->getChildByName("setting");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(GameScene::touchButton));

	UILayer *p = UILayer::create();
	p->addWidget(CCUIHELPER->createWidgetFromJsonFile("Hint/Hint.ExportJson"));
	ActionManager::shareManager()->playActionByName("Hint.ExportJson","Animation0");
	this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(4),
		CCCallFunc::create(this,callfunc_selector(GameScene::start))));
	if ((num %2 ?GAMETYPE_STEP:GAMETYPE_TIME)==GAMETYPE_STEP)
	{
		p->getWidgetByName("step")->setVisible(true);
	}
	else
	{
		p->getWidgetByName("time")->setVisible(true);
	}
	this->addChild(p,100,6);
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

void GameScene::start()
{
	pPlayLayer->startGame(level);
	this->removeChildByTag(6);
}
