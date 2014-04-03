#include "GameScene.h"

USING_NS_CC;


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

	CCLOG("%f %f",pPlayLayer->getPositionX(),pPlayLayer->getPositionY());
    
    return true;
}
