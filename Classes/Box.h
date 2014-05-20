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

	int combo_type;//1������� 2��������
	CCSprite *pSprite;//С���ͼ��
	int _type;//С������
	int x , y;//С��λ��
    // implement the "static node()" method manually
    CREATE_FUNC(Box);
	void swap(Box *a);//����С��
	CCPoint getP();//��ȡС��λ��
	bool check(Box *other = NULL,int dir = HORIZON);//����Ƿ����

	bool removeable;//�Ƿ��ڱ�������

	CCParticleMeteor *getEffectWithRadio(float radio);//����������Ч

	void addEffect(int type);//���������Ч
};

