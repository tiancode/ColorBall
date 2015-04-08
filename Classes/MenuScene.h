#ifndef __MENU_SCENE__
#define __MENU_SCENE__

#include "cocos2d.h"
#include <vector>

#define TOPLEVEL  10000

class MenuScene : public cocos2d::Layer
{
public:
	MenuScene();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MenuScene);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
	void updateScene(float dt);

	cocos2d::Size m_visibleSize;
	cocos2d::Vec2 m_origin;

	int getTopScore();
	int sound();


	cocos2d::Sprite *start;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::Sprite *share;
#endif
	cocos2d::Sprite *soun;
	bool havaSound;

	cocos2d::DrawNode *draw;
	int circleNum;
};



#endif