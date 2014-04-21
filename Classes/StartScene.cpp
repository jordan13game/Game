#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "SelectScene.h"
#include "GameDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;


// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCScene::init() )
    {
        return false;
    }
    

	pLayer = UILayer::create();
	pLayer->addWidget(CCUIHELPER->createWidgetFromJsonFile("Start/Start.ExportJson"));
	this->addChild(pLayer);

	setTag(31);

	UIButton *bt = (UIButton *)pLayer->getWidgetByName("Panel")->getChildByName("start");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(StartScene::touchStart));

	initUserInfo();

    return true;
}

void StartScene::touchStart( cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type )
{
	if (type == TOUCH_EVENT_ENDED)
	{
		SelectScene *pScene = SelectScene::create();
		CCTransitionScene * reScene = NULL;
		reScene = CCTransitionFadeBL::create(1.0f, pScene);
		CCDirector::sharedDirector()->replaceScene(reScene);
	}
}

void StartScene::initUserInfo()
{
	if (!CCUserDefault::sharedUserDefault()->getBoolForKey("isHaveSaveFileXml"))
	{
		CCUserDefault::sharedUserDefault()->setBoolForKey("isHaveSaveFileXml",true);
		for (int i=1;i<=TOTLEVEL;i++)
		{
			CCUserDefault::sharedUserDefault()->setIntegerForKey(CCString::createWithFormat("star%d",i)->m_sString.c_str(),0);
			CCUserDefault::sharedUserDefault()->setIntegerForKey(CCString::createWithFormat("highscore%d",i)->m_sString.c_str(),0);
		}
		
	}

}

void StartScene::keyBackClicked()
{
	exit(0);
}




