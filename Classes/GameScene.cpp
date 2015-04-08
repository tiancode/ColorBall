#include "GameScene.h"

#include "MenuScene.h"
#include "SimpleAudioEngine.h"

#include "AdmobHelper.h"

USING_NS_CC;

int level;
bool havaSound;
Scene* GameScene::createScene(int lev,bool sound)
{
	level = lev;
	havaSound = sound;
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setGravity(Vect(0, 0));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	while (!m_bullutList.empty())
	{
		m_bullutList.pop();
	}

	m_winSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	if (havaSound == true)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Audio/game.mp3",true);
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("Audio/game.mp3");
	}

	m_drawCircle = DrawNode::create();
	m_drawCircle->setAnchorPoint(Vec2(0.5, 0.5));
	m_drawCircle->setPosition(getCenterCircle());
	this->addChild(m_drawCircle);

	char buff[32];
	sprintf(buff, "%d", level);
	LabelTTF *levelNum = LabelTTF::create(buff, "fonts/Marker Felt.ttf", 50);
	levelNum->setPosition(getCenterCircle());
	this->addChild(levelNum, 10);

	m_drawCircle->drawDot(Vec2(0,0), getCircleRandis(), Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

	m_lowerPosition.x = m_drawCircle->getPosition().x;
	m_lowerPosition.y= m_drawCircle->getPosition().y - 3 * getCircleRandis();
	m_bullutPosition1.x = m_drawCircle->getPosition().x;
	m_bullutPosition1.y = m_drawCircle->getPosition().y - 4 * getCircleRandis();
	m_bullutPosition2.x = m_bullutPosition1.x;
	m_bullutPosition2.y = m_bullutPosition1.y - 4*getCircleRandis() / 5;

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto listenerPhy = EventListenerPhysicsContact::create();
	listenerPhy->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	listenerPhy->onContactPreSolve = CC_CALLBACK_2(GameScene::onContactPreSolve, this);
	listenerPhy->onContactPostSolve = CC_CALLBACK_2(GameScene::onContactPostSolve, this);
	listenerPhy->onContactSeperate = CC_CALLBACK_1(GameScene::onContactSeperate, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listenerPhy, 1);

	schedule(schedule_selector(GameScene::updateScene), 6.0f, kRepeatForever, 3);

	play(level);

	int isad = random(0, 1);
	if (isad == 0)
		AdmobHelper::showAd();

	return true;
}


cocos2d::Vec2 GameScene::getCenter()
{
	return Vec2(m_winSize.width / 2, m_winSize.height/2);
}

cocos2d::Vec2 GameScene::getCenterCircle()
{
	return Vec2(getCenter().x, getCenter().y + getCircleRandis()+40);
}

float GameScene::getCircleRandis()
{
	return (m_winSize.width - 60) / 3 / 2;
}

cocos2d::Vec2 GameScene::getBigOutCirclePoint(float angle)
{
	Vec2 center = Vec2(0, 0);
	Vec2 startPoint;
	startPoint.x =  + 3*getCircleRandis();
	startPoint.y = 0;

	return startPoint.rotateByAngle(center, angle);
}



DrawNode* GameScene::random_sademaji()
{
	DrawNode *emaji = DrawNode::create();
	emaji->drawDot(Vec2(0, 0), 32, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
	auto body = PhysicsBody::createCircle(32);
	//body->setGravityEnable(false);
	body->setDynamic(false);
	body->setContactTestBitmask(0x1);
	emaji->setPhysicsBody(body);
	return emaji;
}
DrawNode* GameScene::random_happyemaji()
{
	DrawNode *emaji = DrawNode::create();
	emaji->drawDot(Vec2(0, 0), 32, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
	auto body = PhysicsBody::createCircle(32);
	//body->setGravityEnable(false);
	body->setDynamic(false);
	body->setContactTestBitmask(0x1);
	emaji->setPhysicsBody(body);
	return emaji;
}

void GameScene::setCircleLine(int lineNum)
{
	if (lineNum == 0)
		return;

	float ang = 0.0;
	float deg = 2 * PI / lineNum;
	for (int i = 0; i < lineNum; i++)
	{
		Vec2 point = getBigOutCirclePoint(ang += deg);
		DrawNode *m_drawLine = DrawNode::create();
		m_drawCircle->addChild(m_drawLine, -1);
		m_drawLine->drawLine(Vec2(0, 0),point,
								Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));

		auto *emaji = random_sademaji();
		/*emaji->getPhysicsBody()->setDynamic(true)*/;
		m_drawCircle->addChild(emaji);
		emaji->setPosition(point);
		ActionInterval * actionBy = ScaleTo::create(1, getCircleRandis()*2 /5 /64);
		emaji->runAction(Sequence::create(actionBy, NULL));
		ActionInterval * rotateBy = RotateBy::create(1, 360*cocos2d::random(-1, 1));
		emaji->runAction(RepeatForever::create(rotateBy));
	}
}

void GameScene::setBullut(int bullutNum)
{
	leftBulut = bullutNum;
	char buff[32];
	for (int i = 0; i < bullutNum; i++)
	{
		auto *emaji = random_happyemaji();
		if (i == 0)
		{
			m_bullut1 = emaji;
			m_bullut1->setPosition(m_bullutPosition1);
			m_bullut1->setScale(getCircleRandis() * 2 / 5 / 64);
			//m_bullut1->getPhysicsBody()->setDynamic(true);
			this->addChild(m_bullut1);
			
			sprintf(buff, "%d", leftBulut);
			LabelTTF *levelBullut = LabelTTF::create(buff, "fonts/Marker Felt.ttf", 43);
			levelBullut->setColor(Color3B(random(0, 255), random(0, 255), random(0, 255)));
			m_bullut1->addChild(levelBullut, 10);
			leftBulut--;
			continue;
		}
		if (i == 1)
		{
			m_bullut2 = emaji;
			m_bullut2->setPosition(m_bullutPosition2);
			m_bullut2->setScale(getCircleRandis() * 2 / 5 / 64);
			this->addChild(m_bullut2);
		
			sprintf(buff, "%d", leftBulut);
			LabelTTF *levelBullut = LabelTTF::create(buff, "fonts/Marker Felt.ttf", 43);
			levelBullut->setColor(Color3B(random(0, 255), random(0, 255), random(0, 255)));
			m_bullut2->addChild(levelBullut, 10);
			leftBulut--;
			continue;
		}
		emaji->setPosition(m_winSize.width/2, m_origin.y-64);
		emaji->setScale(getCircleRandis() * 2 / 5 / 64);
		this->addChild(emaji);
	
		sprintf(buff, "%d", leftBulut);
		LabelTTF *levelBullut = LabelTTF::create(buff, "fonts/Marker Felt.ttf", 43);
		levelBullut->setColor(Color3B(random(0, 255), random(0, 255), random(0, 255)));
		emaji->addChild(levelBullut, 10);
		leftBulut--;
		m_bullutList.push(emaji);
	}
}


bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
	if (m_bullut1 != NULL)
	{
		auto actionMoveDone = CallFuncN::create(CC_CALLBACK_1(GameScene::MoveToLowPosDone, this));
		auto moveToLowPos = MoveTo::create(0.1, m_lowerPosition);
		m_bullut1->runAction(Sequence::create(moveToLowPos, actionMoveDone, NULL));
	}
	return true;
}

void GameScene::onTouchMoved(Touch* touch, Event* event)
{

}

void GameScene::updateScene(float dt)
{
	//Íê³É¹Ø¿¨
	if (m_bullut1 == NULL)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("Audio/game.mp3");
		if (havaSound == true)
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/yip.wav");
		UserDefault::getInstance()->setIntegerForKey("TopLevel", ++level);

		AdmobHelper::hideAd();

		auto scene = MenuScene::createScene();
		TransitionScene *transition = TransitionRotoZoom::create(1, scene);
		Director::getInstance()->replaceScene(transition);
	}
}

void GameScene::MoveToLowPosDone(CCNode* node)
{
	if (m_bullut1 != NULL)
	{
		if (havaSound == true)
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/hitted.wav");
		m_bullut1->removeFromParentAndCleanup(false);
		this->removeChild(m_bullut1, false);
		m_bullut1->getPhysicsBody()->setDynamic(true);
		Vec2 lowerpos = m_drawCircle->convertToNodeSpace(m_lowerPosition);
		m_bullut1->setPosition(lowerpos);
		ActionInterval * rotateBy = RotateBy::create(1, 360 * cocos2d::random(-1, 1));
		m_bullut1->runAction(RepeatForever::create(rotateBy));
		m_drawCircle->addChild(m_bullut1);
		m_bullut1 = NULL;

		DrawNode *m_drawLine = DrawNode::create();
		m_drawLine->drawLine(Vec2(0, 0), lowerpos,
								Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
		m_drawCircle->addChild(m_drawLine, -1);
	}

	if (m_bullut2 != NULL)
	{
		auto moveToPos1 = MoveTo::create(0.1, m_bullutPosition1);
		m_bullut2->getPhysicsBody()->setDynamic(true);
		m_bullut2->runAction(moveToPos1);
		m_bullut1 = m_bullut2;
		m_bullut2 = NULL;
	}

	if (!m_bullutList.empty())
	{
		m_bullut2 = m_bullutList.front();
		auto moveToPos2 = MoveTo::create(0.1, m_bullutPosition2);
		m_bullut2->runAction(moveToPos2);
		m_bullutList.pop();
	}
}
void GameScene::onTouchEnded(Touch* touch, Event* event)
{

}

//Ê§°Ü
bool GameScene::onContactBegin(PhysicsContact& contact)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("Audio/game.mp3");
	if (havaSound == true)
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Audio/dead.wav");
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

	AdmobHelper::hideAd();

	auto scene = MenuScene::createScene();
	TransitionScene *transition = TransitionRotoZoom::create(1, scene);
	Director::getInstance()->replaceScene(transition);

	return true;
}

bool GameScene::onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve)
{
	return true;
}

void GameScene::onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve)
{

}

void GameScene::onContactSeperate(PhysicsContact& contact)
{

}

void GameScene::play(int level)
{
	if (level > TOPLEVEL)
	{
		UserDefault::getInstance()->setIntegerForKey("TopLevel", 1);
		playLevel1();
	}
	switch(level)
	{
		case 1:
			playLevel1();
			break;
		case 2:
			playLevel2();
			break;
		case 3:
			playLevel3();
			break;
		case 4:
			playLevel4();
			break;
		case 5:
			playLevel5();
			break;
		case 6:
			playLevel6();
			break;
		case 7:
			playLevel7();
			break;
		case 8:
			playLevel8();
			break;
		case 9:
			playLevel9();
			break;
		case 10:
			playLevel10();
			break;
		case 11:
			playLevel11();
			break;
		case 12:
			playLevel12();
			break;
		case 13:
			playLevel13();
			break;
		case 14:
			playLevel14();
			break;
		case 15:
			playLevel15();
			break;
		case 16:
			playLevel16();
			break;
		case 17:
			playLevel17();
			break;
		case 18:
			playLevel18();
			break;
		case 19:
			playLevel19();
			break;
		case 20:
			playLevel20();
			break;
		case 21:
			playLevel21();
			break;
		case 22:
			playLevel22();
			break;
		case 23:
			playLevel23();
			break;
		case 24:
			playLevel24();
			break;
		case 25:
			playLevel25();
			break;
		case 26:
			playLevel26();
			break;
		case 27:
			playLevel27();
			break;
		case 28:
			playLevel28();
			break;
		case 29:
			playLevel29();
			break;
		case 30:
			playLevel30();
			break;
		case 31:
			playLevel31();
			break;
		case 32:
			playLevel32();
			break;
		case 33:
			playLevel33();
			break;
		case 34:
			playLevel34();
			break;
		case 35:
			playLevel35();
			break;
		case 36:
			playLevel36();
			break;
		case 37:
			playLevel37();
			break;
	}
	if (level >= 38 && level <= 48)
	{
		int linenum = random(1, 12);
		int bulnum = 23 - linenum;
		setCircleLine(linenum);
		setBullut(bulnum);
		setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_NORMOL_CONTER : SPINSPEEDMODE_NORMOL_CLOCKWISE, 4.5);
	}
	if (level >= 49 && level <= 59)
	{
		int linenum = random(1, 12);
		int bulnum = 23 - linenum;
		setCircleLine(linenum);
		setBullut(bulnum);
		setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_TURN_INOUT_CLOCKWISE : SPINSPEEDMODE_TURN_INOUT_CONTER, 4.5);
	}
	if (level >= 60 && level <= 70)
	{
		int linenum = random(1, 12);
		int bulnum = 23 - linenum;
		setCircleLine(linenum);
		setBullut(bulnum);
		setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_NORMOL_CONTER : SPINSPEEDMODE_NORMOL_CLOCKWISE, 4.0);
	}
	if (level >= 71 && level <= 81)
	{
		int linenum = random(1, 12);
		int bulnum = 23 - linenum;
		setCircleLine(linenum);
		setBullut(bulnum);
		setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_TURN_INOUT_CLOCKWISE : SPINSPEEDMODE_TURN_INOUT_CONTER, 4.0);
	}
	if (level > 81)
	{
		int linenum = random(1, 12);
		int bulnum = random(24,27) - linenum;
		setCircleLine(linenum);
		setBullut(bulnum);
		int spp = random(1,7);
		switch (spp)
		{
		case 1:
			setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_NORMOL_CONTER : SPINSPEEDMODE_NORMOL_CLOCKWISE, random(2.5,4.0));
			break;
		case 2:
			setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_TURN_SININOUT_CLOCKWISE : SPINSPEEDMODE_TURN_SININOUT_CONTER, random(2.5, 4.0));
			break;
		case 3:
			setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_TURN_EXPO_CLOCKWISE : SPINSPEEDMODE_TURN_EXPO_CONTER, random(2.5, 4.0));
			break;
		case 4:
			setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_TURN_INOUTBOUNCE_CLOCKWISE : SPINSPEEDMODE_TURN_INOUTBOUNCE_CONTER, random(2.5, 4.0));
			break;
		case 5:
			setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_TURN_INOUTBACK_CLOCKWISE : SPINSPEEDMODE_TURN_INOUTBACK_CONTER, random(2.5, 4.0));
			break;
		case 6:
			setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_TURN_INOUTELASTIC_CLOCKWISE : SPINSPEEDMODE_TURN_INOUTELASTIC_CONTER, random(2.5, 4.0));
			break;
		case 7:
			setCircleSpinSpeed(random(1, 2) ? SPINSPEEDMODE_TURN_INOUT_CLOCKWISE : SPINSPEEDMODE_TURN_INOUT_CONTER, random(2.5, 4.0));
			break;
		}
	}
}

void GameScene::playLevel37()
{
	setCircleLine(3);
	setBullut(8);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 2.5);
}
void GameScene::playLevel36()
{
	setCircleLine(10);
	setBullut(14);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_INOUT_CONTER, 6);
}
void GameScene::playLevel35()
{
	setCircleLine(9);
	setBullut(15);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_INOUT_CLOCKWISE, 6);
}
void GameScene::playLevel34()
{
	setCircleLine(0);
	setBullut(29);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 5.5);
}
void GameScene::playLevel33()
{
	setCircleLine(4);
	setBullut(10);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_INOUT_CONTER, 4);
}
void GameScene::playLevel32()
{
	setCircleLine(3);
	setBullut(7);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_INOUT_CLOCKWISE, 5);
}
void GameScene::playLevel31()
{
	setCircleLine(1);
	setBullut(12);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_INOUTELASTIC_CONTER, 6.5);
}
void GameScene::playLevel30()
{
	setCircleLine(3);
	setBullut(9);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_INOUTELASTIC_CLOCKWISE, 6.5);
}
void GameScene::playLevel29()
{
	setCircleLine(4);
	setBullut(10);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_INOUTBACK_CONTER, 6.5);
}
void GameScene::playLevel28()
{
	setCircleLine(6);
	setBullut(9);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_INOUTBACK_CLOCKWISE, 6.5);
}
void GameScene::playLevel27()
{
	setCircleLine(5);
	setBullut(10);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_INOUTBOUNCE_CONTER, 6.5);
}
void GameScene::playLevel26()
{
	setCircleLine(5);
	setBullut(10);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_INOUTBOUNCE_CLOCKWISE, 6.5);
}
void GameScene::playLevel25()
{
	setCircleLine(0);
	setBullut(30);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 6.5);
}
void GameScene::playLevel24()
{
	setCircleLine(1);
	setBullut(27);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 6.5);
}
void GameScene::playLevel23()
{
	setCircleLine(2);
	setBullut(25);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 6.5);
}
void GameScene::playLevel22()
{
	setCircleLine(3);
	setBullut(23);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 6.5);
}
void GameScene::playLevel21()
{
	setCircleLine(4);
	setBullut(22);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 6.5);
}
void GameScene::playLevel20()
{
	setCircleLine(5);
	setBullut(20);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 6.5);
}
void GameScene::playLevel19()
{
	setCircleLine(6);
	setBullut(6);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_EXPO_CONTER, 4.5);
}
void GameScene::playLevel18()
{
	setCircleLine(6);
	setBullut(6);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_EXPO_CLOCKWISE, 4.5);
}
void GameScene::playLevel17()
{
	setCircleLine(6);
	setBullut(16);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 4.3);
}
void GameScene::playLevel16()
{
	setCircleLine(7);
	setBullut(10);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 4.3);
}
void GameScene::playLevel15()
{
	setCircleLine(9);
	setBullut(14);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 6.5);
}
void GameScene::playLevel14()
{
	setCircleLine(8);
	setBullut(13);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 6.5);
}
void GameScene::playLevel13()
{
	setCircleLine(7);
	setBullut(6);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_SININOUT_CONTER, 4.5);
}
void GameScene::playLevel12()
{
	setCircleLine(6);
	setBullut(6);
	setCircleSpinSpeed(SPINSPEEDMODE_TURN_SININOUT_CLOCKWISE, 4.5);
}
void GameScene::playLevel11()
{
	setCircleLine(9);
	setBullut(12);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 6.4);
}
void GameScene::playLevel10()
{
	setCircleLine(8);
	setBullut(12);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 6.4);
}
void GameScene::playLevel9()
{
	setCircleLine(9);
	setBullut(9);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 6.4);
}
void GameScene::playLevel8()
{
	setCircleLine(8);
	setBullut(10);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 6.4);
}
void GameScene::playLevel7()
{
	setCircleLine(10);
	setBullut(8);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 6.4);
}
void GameScene::playLevel6()
{
	setCircleLine(8);
	setBullut(7);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 6.4);
}
void GameScene::playLevel5()
{
	setCircleLine(7);
	setBullut(6);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 6.4);
}
void GameScene::playLevel4()
{
	setCircleLine(5);
	setBullut(7);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 6.4);
}
void GameScene::playLevel3()
{
	setCircleLine(3);
	setBullut(8);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 6.5);
}
void GameScene::playLevel2()
{
	setCircleLine(2);
	setBullut(6);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CONTER, 6.5);
}
void GameScene::playLevel1()
{
	setCircleLine(1);
	setBullut(6);
	setCircleSpinSpeed(SPINSPEEDMODE_NORMOL_CLOCKWISE, 6.5);
}


void GameScene::setCircleSpinSpeed(SPINSPEEDMODE mode, float speedTime)
{
	if (SPINSPEEDMODE_NORMOL_CLOCKWISE == mode)
	{
		ActionInterval * actionBy = RotateBy::create(speedTime, 360);
		m_drawCircle->runAction(RepeatForever::create(actionBy));
	}
	if (SPINSPEEDMODE_NORMOL_CONTER == mode)
	{
		ActionInterval * actionBy = RotateBy::create(speedTime, -360);
		m_drawCircle->runAction(RepeatForever::create(actionBy));
	}
	if (SPINSPEEDMODE_TURN_SININOUT_CLOCKWISE == mode)
	{
		ActionInterval * actionBy = RotateBy::create(speedTime, 337.5);
		EaseSineInOut *speed = EaseSineInOut::create(actionBy);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_SININOUT_CONTER == mode)
	{
		ActionInterval * actionBy = RotateBy::create(speedTime, -337.5);
		EaseSineInOut *speed = EaseSineInOut::create(actionBy);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_EXPO_CLOCKWISE == mode)
	{
		ActionInterval * expoActionBy = RotateBy::create(speedTime, 337.5);
		EaseExponentialOut *speed = EaseExponentialOut::create(expoActionBy);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_EXPO_CONTER == mode)
	{
		ActionInterval * expoActionBy = RotateBy::create(speedTime, -337.5);
		EaseExponentialOut *speed = EaseExponentialOut::create(expoActionBy);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_INOUTBOUNCE_CLOCKWISE == mode)
	{
		ActionInterval *ActionBy = RotateBy::create(speedTime, 337.5);
		EaseBounceInOut *speed = EaseBounceInOut::create(ActionBy);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_INOUTBOUNCE_CONTER == mode)
	{
		ActionInterval *ActionBy = RotateBy::create(speedTime, -337.5);
		EaseBounceInOut *speed = EaseBounceInOut::create(ActionBy);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_INOUTBACK_CLOCKWISE == mode)
	{
		ActionInterval *ActionBy = RotateBy::create(speedTime, 337.5);
		EaseBackInOut *speed = EaseBackInOut::create(ActionBy);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_INOUTBACK_CONTER == mode)
	{
		ActionInterval *ActionBy = RotateBy::create(speedTime, -337.5);
		EaseBackInOut *speed = EaseBackInOut::create(ActionBy);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_INOUTELASTIC_CLOCKWISE == mode)
	{
		ActionInterval *ActionBy = RotateBy::create(speedTime, 337.5);
		EaseElasticInOut *speed = EaseElasticInOut::create(ActionBy);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_INOUTELASTIC_CONTER == mode)
	{
		ActionInterval *ActionBy = RotateBy::create(speedTime, -337.5);
		EaseElasticInOut *speed = EaseElasticInOut::create(ActionBy);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_INOUT_CLOCKWISE == mode)
	{
		ActionInterval *ActionBy = RotateBy::create(speedTime, 337.5);
		EaseInOut *speed = EaseInOut::create(ActionBy,2);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
	if (SPINSPEEDMODE_TURN_INOUT_CONTER == mode)
	{
		ActionInterval *ActionBy = RotateBy::create(speedTime, -337.5);
		EaseInOut *speed = EaseInOut::create(ActionBy, 2);
		auto repeat = RepeatForever::create(speed);
		m_drawCircle->runAction(repeat);
	}
}

