#include "SettingScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "SelectScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;


// on "init" you need to initialize your instance
bool SettingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCScene::init() )
    {
        return false;
    }
    
	CCScene *p = CCDirector::sharedDirector()->getRunningScene();

	CCSize sz = CCDirector::sharedDirector()->getWinSize();

	CCRenderTexture *tex = CCRenderTexture::create(sz.width,sz.height);

	tex->begin();
	p->visit();
	tex->end();

	CCSprite *spr = CCSprite::createWithTexture(tex->getSprite()->getTexture());
	spr->setPosition(ccp(sz.width/2,sz.height/2));
	spr->setColor(ccGRAY);
	spr->setFlipY(true);
	this->addChild(spr);

	pLayer = UILayer::create();
	pLayer->addWidget(CCUIHELPER->createWidgetFromJsonFile("Setting/Setting.ExportJson"));
	this->addChild(pLayer);

	setTag(31);

	UIButton *bt = (UIButton *)pLayer->getWidgetByName("Panel")->getChildByName("music");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(SettingScene::touchMusicButton));
    
	bt = (UIButton *)pLayer->getWidgetByName("Panel")->getChildByName("soundeffect");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(SettingScene::touchEffectButton));

	bt = (UIButton *)pLayer->getWidgetByName("Panel")->getChildByName("close");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(SettingScene::touchClose));

	bt = (UIButton *)pLayer->getWidgetByName("Panel")->getChildByName("return");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(SettingScene::touchReturn));

	bt = (UIButton *)pLayer->getWidgetByName("Panel")->getChildByName("continue");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(SettingScene::touchClose));

	bt = (UIButton *)pLayer->getWidgetByName("Panel")->getChildByName("restart");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(SettingScene::touchRestart));

    return true;
}



void SettingScene::touchMusicButton(CCObject* obj,TouchEventType type )
{
	//CCLOG("fuck");
	if (type == TOUCH_EVENT_ENDED)
	{
		UIButton *bt = (UIButton *)obj;
		if (bt->isBright())
		{
			bt->setBright(false);
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		}
		else
		{
			bt->setBright(true);
			SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		}
		
		
	}
	
}

void SettingScene::touchEffectButton( cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type )
{
	if (type == TOUCH_EVENT_ENDED)
	{
		UIButton *bt = (UIButton *)obj;
		if (bt->isBright())
		{
			bt->setBright(false);
			SimpleAudioEngine::sharedEngine()->stopAllEffects();
		}
		else
		{
			bt->setBright(true);
			SimpleAudioEngine::sharedEngine()->resumeAllEffects();
		}


	}
}

void SettingScene::touchClose( cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type )
{
	if (type == TOUCH_EVENT_ENDED)
	{
		CCDirector::sharedDirector()->popScene();

	}
}

void SettingScene::touchRestart( cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type )
{
	if (type == TOUCH_EVENT_ENDED)
	{
		CCDirector::sharedDirector()->popScene();
		GameScene *pScene = GameScene::create();
		UILabel* lab =  (UILabel*)pLayer->getWidgetByName("Panel")->getChildByName("num");
		int num;
		sscanf(lab->getStringValue(),"%d",&num);
		CCTransitionScene * reScene = NULL;
		reScene = CCTransitionFadeBL::create(1.0f, pScene);
		CCDirector::sharedDirector()->replaceScene(reScene);
		pScene->loadLevel(num);

	}
}

void SettingScene::touchReturn( cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type )
{
	if (type == TOUCH_EVENT_ENDED)
	{
		CCDirector::sharedDirector()->popScene();
		SelectScene *pScene = SelectScene::create();
		CCTransitionScene * reScene = NULL;
		reScene = CCTransitionFadeBL::create(1.0f, pScene);
		CCDirector::sharedDirector()->replaceScene(reScene);
	}
}

void SettingScene::setLev( int num )
{
	UILabel* lab =  (UILabel*)pLayer->getWidgetByName("Panel")->getChildByName("num");
	lab->setText(CCString::createWithFormat("%d",num)->m_sString.c_str());
}
