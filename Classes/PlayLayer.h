#pragma once

#include "cocos2d.h"
#include "GameDefine.h"
#include "Box.h"

class PlayLayer : public cocos2d::CCLayer
{

	bool islock;
public:
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static node()" method manually
    CREATE_FUNC(PlayLayer);


	cocos2d::CCArray *m_content;
	cocos2d::CCArray *m_toRemove;
	cocos2d::CCArray *m_toJudge;
	cocos2d::CCSprite *bg;
	PlayLayer();
	~PlayLayer();
	bool checkBox();
	int repair();
	int repairSingleCol(int col);
	Box * getBoxAtPosXY(int x,int y);
	Box * boundBox;
	Box * m_seteledBox;

	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	bool swapTwoBoxes(int dx,int dy);

	void removeall();

	void removeSprite(CCObject *p);

	void lock();
	void unlock();

	void judgeAndRepair();

	void addToRemove(Box *p);

	int Boxtype[GRID_WIDTH][GRID_HEIGHT];
};

