#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameDefine.h"
#include "Box.h"

class PlayLayer : public cocos2d::CCLayer
{

	bool islock; //是否锁住操作
public:
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static node()" method manually
    CREATE_FUNC(PlayLayer);


	cocos2d::CCArray *m_content; //小球存放
	cocos2d::CCArray *m_toRemove; //待移除的小球
	cocos2d::CCArray *m_toJudge; //待判断是否可消除的小球
	cocos2d::CCArray *m_toHint; //提示可以消除的小球
	cocos2d::CCSprite *bg; //背景

	int GameLeft; //剩余步数或者时间
	int GameType; //游戏类型 步数或者时间
	int GameTot; //总时间或者步数

	int level; //关卡号

	int totScore; //总分
	int nowScore; //当前消除小球能得分数 50 100...
	int targetScore; //目标分数
	void resetScore(float det); //重置分数 50 100...

	PlayLayer();
	~PlayLayer();
	int repair(); //修复空位
	int repairSingleCol(int col); //修复一列
	Box * getBoxAtPosXY(int x,int y); //获取x行y列的小球
	Box * boundBox; //边界小球
	Box * m_seteledBox; //被触摸到的小球

	virtual void registerWithTouchDispatcher(); //注册触摸事件
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent); //触摸开始
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); //触摸移动
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent); //触摸结束

	bool swapTwoBoxes(int dx,int dy); //判断是否交换两个小球

	void removeall(); //移除所有小球

	void removeSprite(CCObject *p); //移除一个小球

	void lock(); //锁定
	void unlock(); //解锁 继续判断是否可消

	void judgeAndRepair(); //判断和修复

	void addToRemove(Box *p); //添加消除小球

	void addHint(float det); //提示

	bool checkAble(); //检查是否还有可消的 无就重构

	void removeAllBoxes(); //销毁所有小球

	void rebuildAll();//重构所有小球

	void reduce(float det);//步数或者时间递减

	void endGame();//游戏结束

	void startGame(int num);//开始游戏

	void removeFont(cocos2d::CCNode *sender); //移除跳出来的分数

	void updateScore(); //更新分数

	void touchRestart(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type); //按下重新开始
	void touchReturn(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type); //按下返回
	void touchNext(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type);//按下下一关
};

