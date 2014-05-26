#include "PlayLayer.h"
#include "Box.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace CocosDenshion;




// on "init" you need to initialize your instance
bool PlayLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCLOG("%f %f %f %f",visibleSize.height,visibleSize.width,origin.x,origin.y);
	bg = CCSprite::create("background3.jpg");
	bg->setAnchorPoint(ccp(0,0));

	this->addChild(bg);
	
	for (int i = 0 ;i < GRID_WIDTH ;i ++)
	{
		CCArray *row = CCArray::createWithCapacity(GRID_HEIGHT);
		m_content->addObject(row);

		for (int j = 0; j < GRID_HEIGHT ; j++)
		{
			Box * box = Box::create();  
			box->x = i;
			box->y = j;
			row->addObject(box);
		}

	}
	this->setTouchEnabled(true);
	totScore = 0;
	nowScore = 0;

    return true;
}



bool PlayLayer::checkBox()
{
	for (int i = 0 ;i < GRID_HEIGHT ;i ++)
	{
		for (int j = 0; j < GRID_WIDTH ; j++)
		{
			CCArray *tmp = CCArray::create();
		}

	}
	return true;
}

PlayLayer::~PlayLayer()
{
	m_content->release();
	m_content = NULL;
	boundBox->release();
	boundBox = NULL;
	m_toRemove->release();
	m_toRemove = NULL;
	m_toJudge->release();
	m_toJudge = NULL;
	m_toHint->release();
	m_toHint = NULL;
}

PlayLayer::PlayLayer()
{
	m_content = CCArray::createWithCapacity(GRID_WIDTH);
	m_content->retain();
	boundBox = Box::create();
	boundBox->retain();
	boundBox->x = -1;
	boundBox->y = -1;
	m_toRemove = CCArray::create();
	m_toRemove->retain();
	m_toJudge = CCArray::create();
	m_toJudge->retain();
	m_toHint = CCArray::create();
	m_toHint->retain();
	islock = false;
}

int PlayLayer::repairSingleCol( int col )
{
	int num = 0;
	for (int i = 0 ;i < GRID_HEIGHT ; i++)
	{
		Box * tmp = getBoxAtPosXY(col,i);
		if (tmp->_type == -1)
		{
			num++;
		}
		else if (num)
		{
			Box * dest = getBoxAtPosXY(col , i - num);
			tmp->pSprite->stopAllActions();
			tmp->pSprite->runAction(CCMoveBy::create(BOXMOVETIME * num , ccp(0,-BOXSIZE * num)));
			dest->pSprite = tmp->pSprite;
			dest->_type = tmp->_type;
			dest->combo_type = tmp->combo_type;
			m_toJudge->addObject(dest);
		}
	}

	for (int i = 0 ;i<num ; i++)
	{
		Box *tmp = getBoxAtPosXY(col,GRID_HEIGHT - num + i);
		int type = rand()%MAX_NORMALBOX_NUM;
		CCSprite* pSprite = CCSprite::create(BOX_NAME[type]);
		tmp->_type = type;
		tmp->combo_type = 0;
		tmp->pSprite = pSprite;
		this->addChild(pSprite);
		pSprite->setPosition(ccp(GRID_LEFT + (0.5 + col) * BOXSIZE , GRID_BUTTON + (GRID_HEIGHT + 0.5 + i) * BOXSIZE));
		pSprite->runAction(CCMoveBy::create(BOXMOVETIME * num , ccp(0, -num * BOXSIZE)));
		m_toJudge->addObject(tmp);
		//pSprite->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(BOXMOVETIME * num , ccp(0, -num * BOXSIZE)),
		//	CCCallFunc::create(this,callfunc_selector(tmp->check))));
	}

	return num;
}

int PlayLayer::repair()
{
	int maxnum = 0;
	for (int i=0;i<GRID_WIDTH ; i++)
	{
		int tmp = repairSingleCol(i);
		if (tmp > maxnum)
		{
			maxnum = tmp;
		}
		
	}
	return maxnum;
	
}

Box * PlayLayer::getBoxAtPosXY( int x,int y )
{
	if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
	{
		return boundBox;
	}
	
	return (Box *)((CCArray *)m_content->objectAtIndex(x))->objectAtIndex(y);
}

bool PlayLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(islock) return false;

	int x = (pTouch->getLocation().x - GRID_LEFT) / BOXSIZE;
	int y = (pTouch->getLocation().y - GRID_BUTTON) / BOXSIZE;

	m_seteledBox = getBoxAtPosXY(x,y);
	if (m_seteledBox == boundBox)
	{
		m_seteledBox = NULL;
	}
	return true;
}

void PlayLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	if (m_seteledBox != NULL)
	{
		if (m_toHint->count())
		{
			CCObject *p;
			CCARRAY_FOREACH(m_toHint,p)
			{
				CCSprite *t = (CCSprite *)p;
				t->stopActionByTag(1);
				t->setVisible(true);
			}
			m_toHint->removeAllObjects();
		}
		
		this->unschedule(schedule_selector(PlayLayer::addHint));

		CCPoint sub = ccpSub(pTouch->getLocation(),m_seteledBox->getP());
		int dx = sub.x / BOXSIZE, dy = sub.y / BOXSIZE;
		if (dx || dy)
		{
			if (swapTwoBoxes(dx, dy))
			{
				lock();
				removeall();
				this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(BOXDISTIME*1.5),
					CCCallFunc::create(this,callfunc_selector(PlayLayer::judgeAndRepair))));
			}
			m_seteledBox = NULL;
		}
		
	}
}

void PlayLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	this->scheduleOnce(schedule_selector(PlayLayer::addHint),HINT_DELAY);
		
}

void PlayLayer::registerWithTouchDispatcher()
{
	CCDirector *pDirector=CCDirector::sharedDirector();  
	pDirector->getTouchDispatcher()->addTargetedDelegate(this,0,true);  
}

bool PlayLayer::swapTwoBoxes( int dx,int dy )
{
	if (dx < -1) dx = -1;
	if (dy < -1) dy = -1;
	if (dx > 1) dx = 1;
	if (dy > 1) dy = 1;
	if (dx) dy = 0;
	Box * otherBox = getBoxAtPosXY(m_seteledBox->x + dx,m_seteledBox->y + dy);
	if (otherBox == boundBox) return false;
	m_seteledBox->pSprite->runAction(CCMoveBy::create(BOXSWAPTIME,ccp(BOXSIZE * dx , BOXSIZE * dy)));
	otherBox->pSprite->runAction(CCMoveBy::create(BOXSWAPTIME,ccp(BOXSIZE * -dx , BOXSIZE * -dy)));

	bool bRet = false;
	m_seteledBox->swap(otherBox);
	if (m_seteledBox->check(otherBox,dx?HORIZON:VERTICAL)) bRet = true;
	if (otherBox->check(m_seteledBox,dx?HORIZON:VERTICAL)) bRet = true;
	if (bRet == false)
	{
		m_seteledBox->swap(otherBox);
		m_seteledBox->pSprite->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(BOXSWAPTIME*1.5),
			CCMoveBy::create(BOXSWAPTIME,ccp(BOXSIZE * -dx , BOXSIZE * -dy))));
		otherBox->pSprite->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(BOXSWAPTIME*1.5),
			CCMoveBy::create(BOXSWAPTIME,ccp(BOXSIZE * dx , BOXSIZE * dy))));
	}
	return bRet;
}

void PlayLayer::removeall()
{
	nowScore += SCORE;
	CCObject *obj;
	CCARRAY_FOREACH(m_toRemove,obj)
	{
		Box *p = (Box *)obj;
		if (p->removeable)
		{
			p->pSprite->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(BOXDISTIME),
				CCCallFuncO::create(this,callfuncO_selector(PlayLayer::removeSprite),p)));
		}
		else
		{
			p->pSprite->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(BOXDISTIME,0,0),
				CCCallFuncO::create(this,callfuncO_selector(PlayLayer::removeSprite),p)));
		}
		
	}
	m_toRemove->removeAllObjects();
	if (CCUserDefault::sharedUserDefault()->getBoolForKey("Effect"))
	{
		SimpleAudioEngine::sharedEngine()->playEffect("bite.mp3");
	}
	
	
}

void PlayLayer::removeSprite( CCObject *p )
{
	Box *tmp = (Box *)p;
	totScore += nowScore;
	updateScore();
	CCLabelTTF *f = CCLabelTTF::create("0","fonts/Marker Felt.ttf",50);
	CCString *str = CCString::createWithFormat("%d",nowScore);
	f->setString(str->m_sString.c_str());
	f->setPosition(tmp->getP());
	this->addChild(f);
	f->runAction(CCSequence::createWithTwoActions(CCMoveBy::create(BOXDISTIME,ccp(0,20)),
		CCCallFuncN::create(this,callfuncN_selector(PlayLayer::removeFont))));
	if (!tmp->removeable)
	{
		this->removeChild(tmp->pSprite,true);
		tmp->_type = -1;
	}
	else
	{
		tmp->removeable = false;
	}
	
	
}

void PlayLayer::lock()
{
	islock = true;
	this->unschedule(schedule_selector(PlayLayer::resetScore));
}

void PlayLayer::unlock()
{
	bool bRet = false;
	if (m_toJudge->count())
	{
		CCObject *p;
		CCARRAY_FOREACH(m_toJudge,p)
		{
			Box *tmp = (Box*)p;
			if(tmp->check())bRet = true;
		}
	}
	if (bRet)
	{
		removeall();
		this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(BOXDISTIME*1.5),
			CCCallFunc::create(this,callfunc_selector(PlayLayer::judgeAndRepair))));
		return;
	}
	
	if (!checkAble())
	{
		removeAllBoxes();
		this->runAction(CCSequence::create(CCDelayTime::create(BOXDISTIME*1.5),
			CCCallFunc::create(this,callfunc_selector(PlayLayer::rebuildAll)),
			CCDelayTime::create(BOXDISTIME*1.5),
			CCCallFunc::create(this,callfunc_selector(PlayLayer::unlock)),
			NULL));
		return;
	}
	
	if (GameType == GAMETYPE_STEP && islock == true)
	{
		reduce(0);
	}

	islock = false;


	this->scheduleOnce(schedule_selector(PlayLayer::resetScore),SCORE_DELAY);
	

	this->scheduleOnce(schedule_selector(PlayLayer::addHint),HINT_DELAY);

}

void PlayLayer::judgeAndRepair()
{
	int num = repair();
	//getBoxAtPosXY(2,2)->_type=1;
	//getBoxAtPosXY(2,2)->pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(BOX_NAME[1]));
	//getBoxAtPosXY(2,3)->_type=1;
	//getBoxAtPosXY(2,3)->pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(BOX_NAME[1]));
	//getBoxAtPosXY(1,4)->_type=1;
	//getBoxAtPosXY(1,4)->pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(BOX_NAME[1]));
	//getBoxAtPosXY(0,4)->_type=1;
	//getBoxAtPosXY(0,4)->pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(BOX_NAME[1]));
	//getBoxAtPosXY(3,4)->_type=1;
	//getBoxAtPosXY(3,4)->pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(BOX_NAME[1]));
	this->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(num*BOXMOVETIME + 0.03f),
		CCCallFunc::create(this,callfunc_selector(PlayLayer::unlock))));
}

void PlayLayer::addToRemove( Box *p )
{
	if (p == boundBox)
	{
		return ;
	}
	

	if (!m_toRemove->containsObject(p))
	{
		m_toRemove->addObject(p);
		if (p->removeable)return;
		
		if (p->combo_type == COMBO_TYPE_COL)
		{
			for (int i=0;i<GRID_HEIGHT;i++)
			{
				addToRemove(getBoxAtPosXY(p->x,i));
			}
			
		}
		else if (p->combo_type == COMBO_TYPE_ROW)
		{
			for (int i=0;i<GRID_WIDTH;i++)
			{
				addToRemove(getBoxAtPosXY(i,p->y));
			}
			
		}
		else if (p->combo_type == COMBO_TYPE_SQR)
		{
			for (int i=-1;i<=1;i++)
			{
				for (int j=-1;j<=1;j++)
				{
					addToRemove(getBoxAtPosXY(p->x+i,p->y+j));
				}
				
			}
			
		}
		
		
	}
	
}

void PlayLayer::addHint( float det )
{
	if (m_toHint->count())
	{
		m_toHint->removeAllObjects();
	}

	checkAble();
	
	
	CCObject *p;
	CCARRAY_FOREACH(m_toHint,p)
	{
		CCRepeatForever *bli = CCRepeatForever::create(CCBlink::create(1,3));
		bli->setTag(1);
		CCSprite *tmp = (CCSprite *)p;
		tmp->stopAllActions();
		tmp->runAction(bli);
	}
}

bool PlayLayer::checkAble()
{
	int dx[][2]={0,0,-1,-2,1,2,2,3,1,1,1,1,0,0,-1,-2,1,2,-2,-3,-1,-1,-1,-1};
	int dy[][2]={-2,-3,-1,-1,-1,-1,0,0,-1,-2,1,2,2,3,1,1,1,1,0,0,-1,-2,1,2};

	for (int i=0;i<GRID_WIDTH;i++)
	{
		for (int j=0;j<GRID_HEIGHT;j++)
		{
			Box *now = getBoxAtPosXY(i,j);
			for (int k=0;k<24;k++)
			{
				Box *a = getBoxAtPosXY(i+dx[k][0],j+dy[k][0]);
				Box *b = getBoxAtPosXY(i+dx[k][1],j+dy[k][1]);

				if (a==boundBox||b==boundBox)
				{
					continue;
				}

				if (now->_type==a->_type&&now->_type==b->_type)
				{
					m_toHint->addObject(now->pSprite);
					m_toHint->addObject(a->pSprite);
					m_toHint->addObject(b->pSprite);

					
					return true;
				}


			}

		}

	}

	return false;
}

void PlayLayer::removeAllBoxes()
{
	for (int i=0;i<GRID_WIDTH;i++)
	{
		for (int j=0;j<GRID_HEIGHT;j++)
		{
			Box *tmp = getBoxAtPosXY(i,j);
			if (tmp->combo_type == COMBO_TYPE_NONE)
			{
				tmp->pSprite->stopAllActions();
				tmp->pSprite->runAction(CCScaleTo::create(BOXDISTIME,0,0));
			}
		}
		
	}
	
}

void PlayLayer::rebuildAll()
{
	for (int i=0;i<GRID_WIDTH;i++)
	{
		for (int j=0;j<GRID_HEIGHT;j++)
		{
			Box *tmp = getBoxAtPosXY(i,j);
			if (tmp->combo_type == COMBO_TYPE_NONE)
			{
				int type = rand()%MAX_NORMALBOX_NUM;
				tmp->pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(BOX_NAME[type]));
				tmp->_type = type;
				tmp->pSprite->stopAllActions();
				tmp->pSprite->runAction(CCScaleTo::create(BOXDISTIME,1,1));
			}
			m_toJudge->addObject(tmp);
		}

	}
}

void PlayLayer::reduce( float det )
{
	GameLeft--;
	UILayer *player = (UILayer *)this->getParent()->getChildByTag(1);
	UILoadingBar *p = (UILoadingBar *)player->getWidgetByName("redBar");
	p->setPercent(GameLeft * 100 / GameTot);
	if (GameLeft == 0)
	{
		endGame();
	}
	
}

void PlayLayer::endGame()
{
	if (GameType == GAMETYPE_TIME)
	{
		this->unschedule(schedule_selector(PlayLayer::reduce));
	}
	CCScene *ppp = CCDirector::sharedDirector()->getRunningScene();

	CCSize sz = CCDirector::sharedDirector()->getWinSize();

	CCRenderTexture *tex = CCRenderTexture::create(sz.width,sz.height);

	tex->begin();
	ppp->visit();
	tex->end();

	CCSprite *spr = CCSprite::createWithTexture(tex->getSprite()->getTexture());
	spr->setPosition(ccp(sz.width/2,sz.height/2));
	spr->setColor(ccGRAY);
	spr->setFlipY(true);
	this->getParent()->addChild(spr);

	UILayer *p = UILayer::create();
	p->addWidget(CCUIHELPER->createWidgetFromJsonFile("EndGame/EndGame.ExportJson"));
	this->getParent()->addChild(p);
	ActionManager::shareManager()->playActionByName("EndGame.ExportJson","Animation0");

	UIButton *bt = (UIButton *)p->getWidgetByName("Panel")->getChildByName("return");
	bt->setTouchEnabled(true);
	bt->addTouchEventListener(this,toucheventselector(PlayLayer::touchReturn));


	UILabelAtlas *pp = (UILabelAtlas *)p->getWidgetByName("nowscore");
	pp->setStringValue(CCString::createWithFormat("%d",totScore)->m_sString.c_str());

	pp = (UILabelAtlas *)p->getWidgetByName("highestscore");
	int high = CCUserDefault::sharedUserDefault()->getIntegerForKey(CCString::createWithFormat("highscore%d",level)->m_sString.c_str());
	high = max(high,totScore);
	pp->setStringValue(CCString::createWithFormat("%d",high)->m_sString.c_str());

	CCUserDefault::sharedUserDefault()->setIntegerForKey(CCString::createWithFormat("highscore%d",level)->m_sString.c_str(),high);

	int star = CCUserDefault::sharedUserDefault()->getIntegerForKey(CCString::createWithFormat("star%d",level)->m_sString.c_str());
	int nowstar = min(totScore/targetScore,3);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(CCString::createWithFormat("star%d",level)->m_sString.c_str(),max(star,nowstar));

	for (int i=0;i<nowstar;i++)
	{
		p->getWidgetByName(CCString::createWithFormat("star%d",i)->m_sString.c_str())->setVisible(true);
	}

	UILabel *o = (UILabel *)p->getWidgetByName("num");
	o->setText(CCString::createWithFormat("%d",level)->m_sString.c_str());

	if (nowstar)
	{
		bt = (UIButton *)p->getWidgetByName("next");
		bt->setTouchEnabled(true);
		bt->addTouchEventListener(this,toucheventselector(PlayLayer::touchNext));
		bt->setVisible(true);
	}
	else
	{
		bt = (UIButton *)p->getWidgetByName("restart");
		bt->setTouchEnabled(true);
		bt->addTouchEventListener(this,toucheventselector(PlayLayer::touchRestart));
		bt->setVisible(true);
	}
	
	

}

void PlayLayer::startGame( int num )
{
	level = num;
	GameLeft = GameTot = 60 - num * 2;
	GameType = num %2 ?GAMETYPE_STEP:GAMETYPE_TIME;
	targetScore = 2000 * num;
	UILayer *player = (UILayer *)this->getParent()->getChildByTag(1);
	UILabelAtlas *p = (UILabelAtlas *)player->getWidgetByName("TargetScore");
	p->setStringValue(CCString::createWithFormat("%d",targetScore)->m_sString.c_str());
	if (GameType == GAMETYPE_TIME)
	{
		this->schedule(schedule_selector(PlayLayer::reduce),1);
	}
	else if (GameType == GAMETYPE_STEP)
	{

	}

	judgeAndRepair();
}

void PlayLayer::resetScore( float det )
{
	nowScore = 0;
}

void PlayLayer::removeFont( cocos2d::CCNode *sender )
{
	this->removeChild(sender);
}

void PlayLayer::updateScore()
{
	UILayer *player = (UILayer *)this->getParent()->getChildByTag(1);
	UILabelAtlas *p = (UILabelAtlas *)player->getWidgetByName("NowScore");
	p->setStringValue(CCString::createWithFormat("%d",totScore)->m_sString.c_str());
	UILoadingBar *bar = (UILoadingBar *)player->getWidgetByName("blueBar");
	bar->setPercent(min(100,totScore*100/(targetScore*3)));
	for (int i=1;i<=3;i++)
	{
		if (totScore>=targetScore*i && player->getWidgetByName(CCString::createWithFormat("StarGray%d",i)->m_sString.c_str())->isVisible())
		{
			player->getWidgetByName(CCString::createWithFormat("StarGray%d",i)->m_sString.c_str())->setVisible(false);
			player->getWidgetByName(CCString::createWithFormat("Star%d",i)->m_sString.c_str())->setVisible(true);
			UIImageView *t = (UIImageView *)player->getWidgetByName(CCString::createWithFormat("Star%d",i)->m_sString.c_str());
			CCParticleExplosion *tmp = CCParticleExplosion::createWithTotalParticles(40);
			tmp->setStartColor(ccc4f(1,1,1,1));
			tmp->setEndColor(ccc4f(1,1,1,1));
			tmp->setSpeed(100);
			tmp->setStartSize(50);
			tmp->setPosition(0,0);
			//tmp->setDuration(1);
			t->addCCNode(tmp);
		}
		
	}
	
}

void PlayLayer::touchRestart( cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type )
{
	if (type == TOUCH_EVENT_ENDED)
	{
		GameScene *pScene = GameScene::create();
		CCTransitionScene * reScene = NULL;
		reScene = CCTransitionFadeBL::create(1.0f, pScene);
		CCDirector::sharedDirector()->replaceScene(reScene);
		GameScene *p = (GameScene *)this->getParent();
		pScene->loadLevel(level);
	}
}

void PlayLayer::touchReturn( cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type )
{
	if (type == TOUCH_EVENT_ENDED)
	{
		SelectScene *pScene = SelectScene::create();
		CCTransitionScene * reScene = NULL;
		reScene = CCTransitionFadeBL::create(1.0f, pScene);
		CCDirector::sharedDirector()->replaceScene(reScene);
	}
}

void PlayLayer::touchNext( cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type )
{
	if (type == TOUCH_EVENT_ENDED)
	{
		GameScene *pScene = GameScene::create();
		CCTransitionScene * reScene = NULL;
		reScene = CCTransitionFadeBL::create(1.0f, pScene);
		CCDirector::sharedDirector()->replaceScene(reScene);
		GameScene *p = (GameScene *)this->getParent();
		pScene->loadLevel(p->level+1);
	}
}




















