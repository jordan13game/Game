#include "Box.h"
#include "PlayLayer.h"

USING_NS_CC;


// on "init" you need to initialize your instance
bool Box::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCNode::init() )
    {
        return false;
    }
	pSprite = NULL;
    combo_type = 0;
	_type = -1;
    return true;
}

void Box::swap( Box *a )
{
	std::swap(a->combo_type,this->combo_type);
	std::swap(a->_type,this->_type);
	std::swap(a->pSprite,this->pSprite);
}

CCPoint Box::getP()
{
	return ccp(GRID_LEFT + (0.5 + x) * BOXSIZE , GRID_BUTTON + (0.5 + y) * BOXSIZE);
}

bool Box::check(int px = -1,int py = -1,int dir = HORIZON)
{
	int u,d,l,r;
	PlayLayer *pLayer = (PlayLayer *)pSprite->getParent();
	u=d=l=r=0;
	if (px == -1)
	{
		px=x;py=y;
	}
	if (pLayer->m_toRemove->containsObject(this))
	{
		return true;
	}
	
	for (int i = px + 1;i < GRID_WIDTH ; i++)
	{
		if (pLayer->getBoxAtPosXY(i,py)->_type == _type)
		{
			r++;
		}
		else break;
	}
	for (int i = px - 1;i > 0 ; i--)
	{
		if (pLayer->getBoxAtPosXY(i,py)->_type == _type)
		{
			l++;
		}
		else break;
	}
	for (int i = py + 1;i < GRID_HEIGHT ; i++)
	{
		if (pLayer->getBoxAtPosXY(px,i)->_type == _type)
		{
			u++;
		}
		else break;
	}
	for (int i = py - 1;i > 0 ; i--)
	{
		if (pLayer->getBoxAtPosXY(px,i)->_type == _type)
		{
			d++;
		}
		else break;
	}
	if (l+r >= 2 )
	{
		
		for (int i=1;i<=r;i++)
		{
			pLayer->m_toRemove->addObject(pLayer->getBoxAtPosXY(px+i,py));
		}
		for (int i=1;i<=l;i++)
		{
			pLayer->m_toRemove->addObject(pLayer->getBoxAtPosXY(px-i,py));
		}
		if (r+l>=4)
		{
			combo_type = COMBO_TYPE_ALL;
			pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(BOX_NAME[8]));
		}
		else if (r+l==3)
		{
			combo_type = (dir == HORIZON ? COMBO_TYPE_ROW : COMBO_TYPE_COL);
		}
		else
		{
			pLayer->m_toRemove->addObject(this);
		}
		
		return true;
	}
	if (u+d >= 2 )
	{
		
		for (int i=1;i<=u;i++)
		{
			pLayer->m_toRemove->addObject(pLayer->getBoxAtPosXY(px,py+i));
		}
		for (int i=1;i<=d;i++)
		{
			pLayer->m_toRemove->addObject(pLayer->getBoxAtPosXY(px,py-i));
		}
		
		if (u+d>=4)
		{
			combo_type = COMBO_TYPE_ALL;
			pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(BOX_NAME[8]));
		}
		else if (u+d==3)
		{
			combo_type = (dir == HORIZON ? COMBO_TYPE_ROW : COMBO_TYPE_COL);
		}
		else
		{
			pLayer->m_toRemove->addObject(this);
		}
		return true;
	}
	return false;
}



