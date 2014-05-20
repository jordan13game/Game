#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameDefine.h"
#include "Box.h"

class PlayLayer : public cocos2d::CCLayer
{

	bool islock; //�Ƿ���ס����
public:
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static node()" method manually
    CREATE_FUNC(PlayLayer);


	cocos2d::CCArray *m_content; //С����
	cocos2d::CCArray *m_toRemove; //���Ƴ���С��
	cocos2d::CCArray *m_toJudge; //���ж��Ƿ��������С��
	cocos2d::CCArray *m_toHint; //��ʾ����������С��
	cocos2d::CCSprite *bg; //����

	int GameLeft; //ʣ�ಽ������ʱ��
	int GameType; //��Ϸ���� ��������ʱ��
	int GameTot; //��ʱ����߲���

	int level; //�ؿ���

	int totScore; //�ܷ�
	int nowScore; //��ǰ����С���ܵ÷��� 50 100...
	int targetScore; //Ŀ�����
	void resetScore(float det); //���÷��� 50 100...

	PlayLayer();
	~PlayLayer();
	int repair(); //�޸���λ
	int repairSingleCol(int col); //�޸�һ��
	Box * getBoxAtPosXY(int x,int y); //��ȡx��y�е�С��
	Box * boundBox; //�߽�С��
	Box * m_seteledBox; //����������С��

	virtual void registerWithTouchDispatcher(); //ע�ᴥ���¼�
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent); //������ʼ
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent); //�����ƶ�
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent); //��������

	bool swapTwoBoxes(int dx,int dy); //�ж��Ƿ񽻻�����С��

	void removeall(); //�Ƴ�����С��

	void removeSprite(CCObject *p); //�Ƴ�һ��С��

	void lock(); //����
	void unlock(); //���� �����ж��Ƿ����

	void judgeAndRepair(); //�жϺ��޸�

	void addToRemove(Box *p); //�������С��

	void addHint(float det); //��ʾ

	bool checkAble(); //����Ƿ��п����� �޾��ع�

	void removeAllBoxes(); //��������С��

	void rebuildAll();//�ع�����С��

	void reduce(float det);//��������ʱ��ݼ�

	void endGame();//��Ϸ����

	void startGame(int num);//��ʼ��Ϸ

	void removeFont(cocos2d::CCNode *sender); //�Ƴ��������ķ���

	void updateScore(); //���·���

	void touchRestart(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type); //�������¿�ʼ
	void touchReturn(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type); //���·���
	void touchNext(cocos2d::CCObject* obj,cocos2d::extension::TouchEventType type);//������һ��
};

