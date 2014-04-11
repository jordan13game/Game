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

bool Box::check(Box *other,int dir)
{
	int u,d,l,r;
	PlayLayer *pLayer = (PlayLayer *)pSprite->getParent();
	u=d=l=r=0;
	int px=x;
	int py=y;
	if (pLayer->m_toRemove->containsObject(pLayer->getBoxAtPosXY(px,py)))
	{
		return true;
	}
	if (combo_type == COMBO_TYPE_ALL && other != NULL)
	{
		for (int i=0;i<GRID_WIDTH;i++)
		{
			for (int j=0;j<GRID_HEIGHT;j++)
			{
				Box *p = pLayer->getBoxAtPosXY(i,j);
				if (p->_type == other->_type)
				{
					pLayer->addToRemove(p);
				}
				
			}
			
		}
		
		pLayer->addToRemove(this);
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
	for (int i = px - 1;i >= 0 ; i--)
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
	for (int i = py - 1;i >= 0 ; i--)
	{
		if (pLayer->getBoxAtPosXY(px,i)->_type == _type)
		{
			d++;
		}
		else break;
	}
	if (l+r >= 2 && u+d >=2)
	{
		for (int i=1;i<=r;i++)
		{
			pLayer->addToRemove(pLayer->getBoxAtPosXY(px+i,py));
		}
		for (int i=1;i<=l;i++)
		{
			pLayer->addToRemove(pLayer->getBoxAtPosXY(px-i,py));
		}
		for (int i=1;i<=u;i++)
		{
			pLayer->addToRemove(pLayer->getBoxAtPosXY(px,py+i));
		}
		for (int i=1;i<=d;i++)
		{
			pLayer->addToRemove(pLayer->getBoxAtPosXY(px,py-i));
		}
		combo_type = COMBO_TYPE_SQR;
		return true;
	}
	
	if (l+r >= 2 )
	{
		
		for (int i=1;i<=r;i++)
		{
			pLayer->addToRemove(pLayer->getBoxAtPosXY(px+i,py));
		}
		for (int i=1;i<=l;i++)
		{
			pLayer->addToRemove(pLayer->getBoxAtPosXY(px-i,py));
		}
		if (r+l>=4)
		{
			combo_type = COMBO_TYPE_ALL;
			_type = -2;
			pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(BOX_NAME[8]));
		}
		else if (r+l==3)
		{
			combo_type = (dir == HORIZON ? COMBO_TYPE_ROW : COMBO_TYPE_COL);
		}
		else
		{
			pLayer->addToRemove(pLayer->getBoxAtPosXY(px,py));
		}
		
		return true;
	}
	if (u+d >= 2 )
	{
		
		for (int i=1;i<=u;i++)
		{
			pLayer->addToRemove(pLayer->getBoxAtPosXY(px,py+i));
		}
		for (int i=1;i<=d;i++)
		{
			pLayer->addToRemove(pLayer->getBoxAtPosXY(px,py-i));
		}
		
		if (u+d>=4)
		{
			combo_type = COMBO_TYPE_ALL;
			_type = -2;
			pSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(BOX_NAME[8]));
		}
		else if (u+d==3)
		{
			combo_type = (dir == HORIZON ? COMBO_TYPE_ROW : COMBO_TYPE_COL);
		}
		else
		{
			pLayer->addToRemove(pLayer->getBoxAtPosXY(px,py));
		}
		return true;
	}
	return false;
}



