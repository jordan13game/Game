#pragma once

#include "cocos2d.h"
#include "GameDefine.h"
using namespace cocos2d;


class PlayLayer;

class Box : public cocos2d::CCNode
{
	
protected:
	
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

	int combo_type;//1代表横着 2代表竖着
	CCSprite *pSprite;//小球的图像
	int _type;//小球类型
	int x , y;//小球位置
    // implement the "static node()" method manually
    CREATE_FUNC(Box);
	void swap(Box *a);//交换小球
	CCPoint getP();//获取小球位置
	bool check(Box *other = NULL,int dir = HORIZON);//检查是否可消

	bool removeable;//是否在本轮消除

	CCParticleMeteor *getEffectWithRadio(float radio);//生成粒子特效

	void addEffect(int type);//添加粒子特效
};

