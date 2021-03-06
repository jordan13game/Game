#include "SelectScene.h"
#include "GameScene.h"
#include "StartScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

// on "init" you need to initialize your instance
bool SelectScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCScene::init() )
    {
        return false;
    }
    
	pLayer = UILayer::create();
	pLayer->addWidget(CCUIHELPER->createWidgetFromJsonFile("SelectScene/SelectScene.ExportJson"));
	this->addChild(pLayer);

	pScroll = (UIScrollView *)pLayer->getWidgetByTag(1)->getChildByName("gameScrollView");

	for (int i=1;i<TOTLEVEL;i++)
	{
		pScroll->addChild(getButton(i));
	}
    
	UIButton *bt = (UIButton*)pLayer->getWidgetByName("return");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(SelectScene::touchReturn));
	CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);
    return true;
}

cocos2d::extension::UIButton * SelectScene::getButton( int num )
{
	UILayer *tmp = UILayer::create();
	tmp->addWidget(CCUIHELPER->createWidgetFromJsonFile("LevelButton/LevelButton.ExportJson"));

	UIButton *bt = (UIButton *)tmp->getWidgetByTag(29)->getChildByName("LevelButton");

	char a[10];
	sprintf(a,"%d",num);
	num--;
	bt->getParent()->removeChild(bt,false);
	bt->setTitleText(a);
	bt->setTitleFontSize(50);
	bt->setTitleFontName("΢���ź�");
	bt->setPosition(ccp(50 + num % 3 * 220,pScroll->getInnerContainerSize().height - 200 * (num/3 + 1)));
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(SelectScene::touchButton));

	num = CCUserDefault::sharedUserDefault()->getIntegerForKey(CCString::createWithFormat("star%d",num+1)->m_sString.c_str());

	for (int i=0;i<num;i++)
	{
		sprintf(a,"star%d",i);
		bt->getChildByName(a)->setVisible(true);
	}
	

	return bt;
}

void SelectScene::touchButton(CCObject* obj,TouchEventType type )
{
	//CCLOG("fuck");
	if (type == TOUCH_EVENT_ENDED)
	{
		UIButton *bt = (UIButton *)obj;
		GameScene *pScene = GameScene::create();
		int num;
		sscanf(bt->getTitleText(),"%d",&num);
		CCTransitionScene * reScene = NULL;
		reScene = CCTransitionFadeBL::create(1.0f, pScene);
		CCDirector::sharedDirector()->replaceScene(reScene);
		pScene->loadLevel(num);
		//CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(this);
	}
	
}

void SelectScene::keyBackClicked()
{
	if (CCDirector::sharedDirector()->getRunningScene()->isEqual(this))
	{
		StartScene *pScene = StartScene::create();
		CCTransitionScene * reScene = NULL;
		reScene = CCTransitionFadeBL::create(1.0f, pScene);
		CCDirector::sharedDirector()->replaceScene(reScene);
		//CCDirector::sharedDirector()->getKeypadDispatcher()->removeDelegate(this);
	}
}

void SelectScene::touchReturn( cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type )
{
	if (type == TOUCH_EVENT_ENDED)
	{
		keyBackClicked();
	}
}
