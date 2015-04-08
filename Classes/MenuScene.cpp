#include "MenuScene.h"

#include "SimpleAudioEngine.h"
#include "GameScene.h"

#include "stdlib.h"
#include "time.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "C2DXShareSDK.h"
using namespace cn::sharesdk;
#endif

USING_NS_CC;

MenuScene::MenuScene()
{
	circleNum = 0;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, __Dictionary *shareInfo, __Dictionary *error)
{
	//CCLog("shareResultHandler");
	switch (state) {
	case C2DXResponseStateSuccess:
		C2DXShareSDK::toast("Success");
		break;
	case C2DXResponseStateFail:
		C2DXShareSDK::toast("Fail");
		break;
	default:
		C2DXShareSDK::toast("Cancel");
		break;
	}
}
#endif

Scene* MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = MenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	start = Sprite::create("Images/start.png");
	start->setPosition(Vec2(m_visibleSize.width / 2, m_visibleSize.height / 2+30));
	start->setScale(2);
	this->addChild(start,10);

	std::srand(time(NULL));

	draw = DrawNode::create();
	this->addChild(draw);

	if (sound() == 1)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/menu.mp3",true);
		soun = Sprite::create("Images/sound.png");
		soun->setPosition(Vec2(m_visibleSize.width - 30, m_visibleSize.height - 30));
		soun->setScale(0.2);
		this->addChild(soun, 10);
		havaSound = true;
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		soun = Sprite::create("Images/nosound.png");
		soun->setPosition(Vec2(m_visibleSize.width - 30, m_visibleSize.height - 30));
		soun->setScale(0.2);
		this->addChild(soun, 10);
		havaSound = false;
	}

	char buff[32];
	sprintf(buff, "Level: %d", getTopScore());
	LabelTTF *scoretext = LabelTTF::create(buff, "fonts/Marker Felt.ttf", 40);
	scoretext->setPosition(m_visibleSize.width / 2, m_visibleSize.height - 30);
	this->addChild(scoretext,10);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	share = Sprite::create("Images/shareButton.png");
	share->setPosition(Vec2(m_visibleSize.width / 2, m_visibleSize.height / 3));
	share->setScale(0.2);
	this->addChild(share, 10);
#endif

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(MenuScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MenuScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MenuScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, start);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), soun);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), share);
#endif

	schedule(schedule_selector(MenuScene::updateScene), 0.2f, kRepeatForever, 1);

	return true;
}

void MenuScene::updateScene(float dt)
{
	if (circleNum++ < 150)
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.5);

		draw = DrawNode::create();
		draw->setPosition(Vec2(random(m_origin.x, m_visibleSize.width), random(m_origin.y, m_visibleSize.height)));
		this->addChild(draw);
		ActionInterval * scaleBy = ScaleBy::create(1, random(2.0, 22.0));
		ActionEase *speed;
		switch (random(1, 6))
		{
		case 1:
			speed = EaseBackInOut::create(scaleBy);
			break;
		case 2:
			speed = EaseInOut::create(scaleBy,2);
			break;
		case 3:
			speed = EaseElasticInOut::create(scaleBy);
			break;
		case 4:
			speed = EaseBounceInOut::create(scaleBy);
			break;
		case 5:
			speed = EaseExponentialOut::create(scaleBy);
			break;
		case 6:
			speed = EaseSineInOut::create(scaleBy);
			break;
		}
		
		draw->drawDot(Vec2(0, 0), 3, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
		draw->runAction(speed);
		return;
	}
}

bool MenuScene::onTouchBegan(Touch *touch, Event *event)
{
	// event->getCurrentTarget() 返回 *listener's* sceneGraphPriority 节点.
	auto target = static_cast<Sprite*>(event->getCurrentTarget());

	// 获取当前触控点相对与按钮的位置
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	// 检测点击区域
	if (rect.containsPoint(locationInNode))
	{
		//log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
		target->setOpacity(180);
		return true;
	}
	return false;
}

void MenuScene::onTouchMoved(Touch* touch, Event* event)
{

}

void MenuScene::onTouchEnded(Touch* touch, Event* event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	//log("sprite onTouchesEnded.. ");
	target->setOpacity(255);
	//重新设置 zOrder，改变现实顺序
	if (target == start)
	{
		Director::getInstance()->getScheduler()->setTimeScale(1);
		unschedule(schedule_selector(MenuScene::updateScene));

		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("Audio/menu.mp3");
		if (havaSound == true)
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/button.wav");
		Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

		auto scene = GameScene::createScene(getTopScore(), havaSound);
		TransitionScene *transition = TransitionRotoZoom::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
	if (target == soun)
	{
		if (havaSound == true)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			UserDefault::getInstance()->setIntegerForKey("Sound", 2);
			soun->setTexture("Images/nosound.png");
			havaSound = false;
		}
		else
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/menu.mp3",true);
			UserDefault::getInstance()->setIntegerForKey("Sound", 1);
			soun->setTexture("Images/sound.png");
			havaSound = true;
		}
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (target == share)
	{
		__Dictionary *content = __Dictionary::create();
		//Dictionary可用的Key如下，如果需要用到其它字段，可自行参考Sample中的代码实现：
		// (并不是所有平台都有这些字段，需要参考文档http://wiki.mob.com/Android_%E4%B8%8D%E5%90%8C%E5%B9%B3%E5%8F%B0%E5%88%86%E4%BA%AB%E5%86%85%E5%AE%B9%E7%9A%84%E8%AF%A6%E7%BB%86%E8%AF%B4%E6%98%8E)
		char buff[128];
		sprintf(buff, "#ColorBall# I\'am stuck at level %d.", getTopScore());
		content->setObject(String::create(buff), "content"); //要分享的内容，注意在文档中content对应的是text字段
		//content -> setObject(String::create("icon512.png"), "image");
		//content->setObject(String::create("for title"), "title");
		//content->setObject(String::create("测试描述"), "description");
		//content->setObject(String::create("http://sharesdk.cn"), "url");
		//content->setObject(String::createWithFormat("%d", C2DXContentTypeNews), "type");
		//content->setObject(String::create("http://sharesdk.cn"), "siteUrl");
		//content->setObject(String::create("ShareSDK"), "site");
		//content->setObject(String::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
		//content->setObject(String::create("extInfo"), "extInfo"); //微信分享应用时传给应用的附加信息
		C2DXShareSDK::showShareMenu(NULL, content, cocos2d::Point(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
	}
#endif
}


int MenuScene::getTopScore()
{
	int score = UserDefault::getInstance()->getIntegerForKey("TopLevel");
	if (score == 0)
	{
		UserDefault::getInstance()->setIntegerForKey("TopLevel", 1);
	}
	score = UserDefault::getInstance()->getIntegerForKey("TopLevel");
	if (score > TOPLEVEL)
	{
		score = 1;
		UserDefault::getInstance()->setIntegerForKey("TopLevel", 1);
	}
	return score;
}

int MenuScene::sound()
{
	int sound = UserDefault::getInstance()->getIntegerForKey("Sound");
	if (sound == 0)
	{
		UserDefault::getInstance()->setIntegerForKey("Sound", 1); //有音
	}
	sound = UserDefault::getInstance()->getIntegerForKey("Sound");
	return sound;
}