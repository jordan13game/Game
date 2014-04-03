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
			row->addObject(box);
		}

	}
	
	repair();
    return true;
}

const char* PlayLayer::BOX_NAME[9] = {"bubble_0.png","bubble_1.png","bubble_2.png","bubble_3.png","bubble_4.png"
										,"bubble_5.png","bubble_6.png","bubble_7.png","bubble_8.png"};

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

const float PlayLayer::BOXMOVETIME = 0.05f;
