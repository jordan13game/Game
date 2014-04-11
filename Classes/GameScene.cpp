#include "GameScene.h"


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
    
    return true;
}

void GameScene::loadLevel( int num )
{
	player = UILayer::create();
	player->addWidget(CCUIHELPER->createWidgetFromJsonFile("Head.ExportJson"));
	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	this->addChild(player);
	CCSize tmp=player->getWidgetByName("Panel")->getSize();
	player->setPosition(0,sz.height-tmp.height);
	pPlayLayer->judgeAndRepair();
	UILoadingBar *p = (UILoadingBar *)player->getWidgetByName("redBar");
	p->setPercent(50);

}
