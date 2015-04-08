#include "SplashScene.h"

#include "SimpleAudioEngine.h"

#include "MenuScene.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//ÇÐ»»³¡¾°¶¨Ê±Æ÷
	scheduleOnce(schedule_selector(SplashScene::startMenuSence), 2);

	//ÏÔÊ¾Æô¶¯»­Ãæ
	auto defSprite = Sprite::create("Images/splash2.png");
	defSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(defSprite, 0);

	//»º´æÉùÒô
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audio/game.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Audio/menu.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/yip.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/hitted.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/dead.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Audio/button.wav");
	
	//»º´æ
	CCTextureCache::sharedTextureCache()->addImage("Images/sound.png");
	CCTextureCache::sharedTextureCache()->addImage("Images/nosound.png");
    
    return true;
}

void SplashScene::startMenuSence(float dt)
{
	auto scene = MenuScene::createScene();
	TransitionScene *transition = TransitionZoomFlipAngular::create(1, scene);
	Director::getInstance()->replaceScene(scene);
}
