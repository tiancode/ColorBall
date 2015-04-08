#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164

enum SPINSPEEDMODE
{
	SPINSPEEDMODE_NORMOL_CLOCKWISE,
	SPINSPEEDMODE_NORMOL_CONTER,

	SPINSPEEDMODE_TURN_SININOUT_CLOCKWISE,
	SPINSPEEDMODE_TURN_SININOUT_CONTER,

	SPINSPEEDMODE_TURN_EXPO_CLOCKWISE, 
	SPINSPEEDMODE_TURN_EXPO_CONTER,

	SPINSPEEDMODE_TURN_INOUTBOUNCE_CLOCKWISE,
	SPINSPEEDMODE_TURN_INOUTBOUNCE_CONTER,

	SPINSPEEDMODE_TURN_INOUTBACK_CLOCKWISE,
	SPINSPEEDMODE_TURN_INOUTBACK_CONTER,

	SPINSPEEDMODE_TURN_INOUTELASTIC_CLOCKWISE,
	SPINSPEEDMODE_TURN_INOUTELASTIC_CONTER,

	SPINSPEEDMODE_TURN_INOUT_CLOCKWISE,
	SPINSPEEDMODE_TURN_INOUT_CONTER
};


class GameScene : public cocos2d::Layer
{
public:   
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(int level, bool sound);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);


	void play(int level);
private:
	cocos2d::Size m_winSize;
	cocos2d::Vec2 m_origin;

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	bool onContactBegin(cocos2d::PhysicsContact& contact);
	bool onContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
	void onContactPostSolve(cocos2d::PhysicsContact& contact, const cocos2d::PhysicsContactPostSolve& solve);
	void onContactSeperate(cocos2d::PhysicsContact& contact);

	void updateScene(float dt);

private:
	cocos2d::DrawNode *m_drawCircle;

	cocos2d::Vec2 m_lowerPosition;
	cocos2d::DrawNode* m_bullut1;
	cocos2d::Vec2 m_bullutPosition1;
	cocos2d::DrawNode* m_bullut2;
	cocos2d::Vec2 m_bullutPosition2;
	std::queue<cocos2d::DrawNode*>  m_bullutList;
	void MoveToLowPosDone(cocos2d::CCNode* node);
	void setBullut(int bullutNum);
	int leftBulut;

	cocos2d::Vec2 getCenter();
	float getCircleRandis();
	cocos2d::Vec2 getCenterCircle();

	cocos2d::Vec2 getBigOutCirclePoint(float angle);

	cocos2d::DrawNode* random_sademaji();
	cocos2d::DrawNode* random_happyemaji();

	void setCircleSpinSpeed(SPINSPEEDMODE mode,float speedTime);
	void setCircleLine(int lineNum);

	void playLevel1();
	void playLevel2();
	void playLevel3();
	void playLevel4();
	void playLevel5();
	void playLevel6();
	void playLevel7();
	void playLevel8();
	void playLevel9();
	void playLevel10();
	void playLevel11();
	void playLevel12();
	void playLevel13(); 
	void playLevel14();
	void playLevel15();
	void playLevel16();
	void playLevel17();
	void playLevel18();
	void playLevel19();
	void playLevel20();
	void playLevel21();
	void playLevel22();
	void playLevel23();
	void playLevel24();
	void playLevel25();
	void playLevel26();
	void playLevel27();
	void playLevel28();
	void playLevel29();
	void playLevel30();
	void playLevel31();
	void playLevel32();
	void playLevel33();
	void playLevel34();
	void playLevel35();
	void playLevel36();
	void playLevel37();
};


#endif