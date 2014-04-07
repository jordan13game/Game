#include "PlayLayer.h"
#include "Box.h"

USING_NS_CC;


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
	repair();
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
		CCPoint sub = ccpSub(pTouch->getLocation(),m_seteledBox->getP());
		int dx = sub.x / BOXSIZE, dy = sub.y / BOXSIZE;
		if (dx || dy)
		{
			if (swapTwoBoxes(dx, dy))
			{
				removeall();
				repair();
			}
			m_seteledBox = NULL;
		}
		
		
	}
}

void PlayLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	
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
	if (m_seteledBox->check(m_seteledBox->x + dx,m_seteledBox->y + dy,(dx?HORIZON:VERTICAL))) bRet = true;
	if (otherBox->check(m_seteledBox->x,m_seteledBox->y,(dx?HORIZON:VERTICAL))) bRet = true;
	if (bRet == false)
	{
		m_seteledBox->pSprite->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(BOXSWAPTIME*1.5),
			CCMoveBy::create(BOXSWAPTIME,ccp(BOXSIZE * -dx , BOXSIZE * -dy))));
		otherBox->pSprite->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(BOXSWAPTIME*1.5),
			CCMoveBy::create(BOXSWAPTIME,ccp(BOXSIZE * dx , BOXSIZE * dy))));
	}
	return bRet;
}

void PlayLayer::removeall()
{
	CCObject *obj;
	CCARRAY_FOREACH(m_toRemove,obj)
	{
		Box *p = (Box *)obj;
		p->pSprite->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(BOXDISTIME,0,0),
			CCCallFuncO::create(this,callfuncO_selector(PlayLayer::removeSprite),p)));
	}
}

void PlayLayer::removeSprite( CCObject *p )
{
	Box *tmp = (Box *)p;
	this->removeChild(tmp->pSprite,true);
}


