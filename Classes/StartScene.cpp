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

	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background.mp3",true);
	SimpleAudioEngine::sharedEngine()->preloadEffect("bite.mp3");
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);

	initUserInfo();
	CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);
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
		//CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(this);
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
		CCUserDefault::sharedUserDefault()->setBoolForKey("Sound",true);
		CCUserDefault::sharedUserDefault()->setBoolForKey("Effect",true);
	}
	else
	{
		if (!CCUserDefault::sharedUserDefault()->getBoolForKey("Sound"))
		{
			SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		}


		if (!CCUserDefault::sharedUserDefault()->getBoolForKey("Effect"))
		{
			SimpleAudioEngine::sharedEngine()->pauseAllEffects();
		}

	}
}

void StartScene::keyBackClicked()
{
	if (CCDirector::sharedDirector()->getRunningScene()->isEqual(this))
	{
		exit(0);
	}
}




