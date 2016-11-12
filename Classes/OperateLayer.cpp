#include "OperateLayer.h"
#include "SimpleAudioEngine.h"
#include "PauseMenu.h"
#include "GameLayer.h"
USING_NS_CC;
using namespace CocosDenshion;
int OperateLayer::curBgm = 0;
OperateLayer::OperateLayer() :move1(Point(0,0)),move2(Point(0,0)), angle(0), preTouchPoint(Vec2(0, 0)), currTouchPoint(Vec2(0, 0))
{
}

OperateLayer::~OperateLayer()
{
}

OperateLayer * OperateLayer::create(int bgm, int mode)
{
	OperateLayer* pRet = new(std::nothrow) OperateLayer();
	if (pRet && pRet->init(bgm, mode))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
		}
	}

bool OperateLayer::init(int bgm, int mode)
{
	bool ret = false;
	do {
		CC_BREAK_IF(!Layer::init());
		this->mode = mode;

		auto touchListener2 = EventListenerTouchOneByOne::create() ;  // ������㴥��������
		touchListener2->onTouchBegan = CC_CALLBACK_2(OperateLayer::onTouchBegan, this);
		touchListener2->onTouchMoved = CC_CALLBACK_2(OperateLayer::onTouchMoved, this);
		touchListener2->onTouchEnded = CC_CALLBACK_2(OperateLayer::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener2, this);

		//auto touchesListener = EventListenerTouchAllAtOnce::create();  // 创建多点触摸监听�?
		//touchesListener->onTouchesBegan = CC_CALLBACK_2(OperateLayer::onTouchesBegan, this);
		//touchesListener->onTouchesMoved = CC_CALLBACK_2(OperateLayer::onTouchesMoved, this);
		//touchesListener->onTouchesEnded = CC_CALLBACK_2(OperateLayer::onTouchesEnded, this);
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(touchesListener, this);

		//�������̼�����
		auto keyboardListener = EventListenerKeyboard::create();
		keyboardListener->onKeyPressed = CC_CALLBACK_2(OperateLayer::onKeyPressed, this);
		keyboardListener->onKeyReleased = CC_CALLBACK_2(OperateLayer::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

		auto audio = SimpleAudioEngine::getInstance();
		audio->preloadEffect("music/Fire1.mp3");
		audio->preloadEffect("music/Fire3.mp3");
		scheduleUpdate();


		ret = true;
	} while (false);
	curBgm = bgm;
	return ret;
}

void OperateLayer::updatePercent1(float f)
{   
	auto percent = hero1->playerarrow->getPercentage() + 3;
	hero1->setPower(percent);
}

void OperateLayer::updatePercent2(float f)
{
	auto percent = hero2->playerarrow->getPercentage() + 3;
	hero2->setPower(percent);
}

void OperateLayer::updateAngle(float f)
{
	hero1->rotateArrow(angle);
}




bool OperateLayer::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	if (touch->getLocation().x < origin.x + visibleSize.width / 2) return false;
	if (mode == 1)
	{
		schedule(schedule_selector(OperateLayer::updatePercent1), 0.1f); // ��ʼ����
		currTouchPoint = touch->getLocation();
		if (!currTouchPoint.equals(preTouchPoint)) {
			hero1->rotateArrow(currTouchPoint);
			preTouchPoint = currTouchPoint;
		}
	}
	return true;
}

void OperateLayer::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (mode == 1)
	{
		currTouchPoint = touch->getLocation();
		if (!currTouchPoint.equals(preTouchPoint)) {
			hero1->rotateArrow(currTouchPoint);
			preTouchPoint = currTouchPoint;
		}
	}
}

void OperateLayer::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (mode == 1)
	{
		unschedule(schedule_selector(OperateLayer::updatePercent1));
		SimpleAudioEngine::getInstance()->playEffect("music/Fire1.mp3", false, 1.0f, 1.0f, 1.0f);
		hero1->pAttack();
		hero1->setPower(0);
	}
	else if (mode == 2)
	{
		SimpleAudioEngine::getInstance()->playEffect("music/attack.mp3", false, 1.0f, 1.0f, 1.0f);
		amHero->pAttack();
	}
}

void OperateLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event)
{
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		keys[code] = true;
		move1 = Point(-5, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		keys[code] = true;
		move2 = Point(-5, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		keys[code] = true;
		move1 = Point(5, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		keys[code] = true;
		move2 = Point(5, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		keys[code] = true;
		move1 = Point(0, 5);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		keys[code] = true;
		move2 = Point(0, 5);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		keys[code] = true;
		move1 = Point(0, -5);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		keys[code] = true;
		move2 = Point(0, -5);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		if (mode == 1) {
			keys[code] = true;
			angle += 0.2;
			if (angle > M_PI / 2) angle = M_PI / 2;
			hero1->rotateArrow(angle);
			//schedule(schedule_selector(OperateLayer::updateAngle, 0.1f));
		}

		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		if (mode == 1)
		{
			keys[code] = true;
			angle -= 0.2;
			if (angle < -M_PI / 2) angle = -M_PI / 2;
			hero1->rotateArrow(angle);
			//schedule(schedule_selector(OperateLayer::updateAngle, 0.1f));
		}

		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_9:
		if (mode == 1)
		{
			keys[code] = true;
			angle += 0.2;
			if (angle > M_PI / 2) angle = M_PI / 2;
			hero2->rotateArrow(angle);
		}

		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_8:
		if (mode == 1)
		{
			keys[code] = true;
			angle -= 0.2;
			if (angle < -M_PI / 2) angle = -M_PI / 2;
			hero2->rotateArrow(angle);
		}

		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		if (mode == 1)
		{
			keys[code] = true;
			schedule(schedule_selector(OperateLayer::updatePercent1), 0.1f);
		}

		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_0:
		if (mode == 1)
		{
			keys[code] = true;
			schedule(schedule_selector(OperateLayer::updatePercent2), 0.1f);
		}

		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_I:
		if (mode == 1)
		{
			if (hero1->mpBar->getPercentage() > 0 && hero1->canUseProp)
				hero1->pUseProp();
		}

		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_5:
		if (mode == 1)
		{
			if (hero2->mpBar->getPercentage() > 0 && hero2->canUseProp)
				hero2->pUseProp();
		}

		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_BACKSPACE:
		break;
	default:
		keys[code] = true;
		break;
	}
}

void OperateLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode code, cocos2d::Event * event)
{
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		keys[code] = false;
		if (keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW]) move2 = Point(5, 0);
		else if (keys[EventKeyboard::KeyCode::KEY_UP_ARROW]) move2 = Point(0, 5);
		else if (keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW]) move2 = Point(0, -5);
		else move2 = Point(0, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		keys[code] = false;
		if (keys[EventKeyboard::KeyCode::KEY_D]) move1 = Point(5, 0);
		else if (keys[EventKeyboard::KeyCode::KEY_W]) move1 = Point(0, 5);
		else if (keys[EventKeyboard::KeyCode::KEY_S]) move1 = Point(0, -5);
		else move1 = Point(0, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		keys[code] = false;
		if (keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW]) move2 = Point(-5, 0);
		else if (keys[EventKeyboard::KeyCode::KEY_UP_ARROW]) move2 = Point(0, 5);
		else if (keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW]) move2 = Point(0, -5);
		else move2 = Point(0, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		keys[code] = false;
		if (keys[EventKeyboard::KeyCode::KEY_A]) move1 = Point(-5, 0);
		else if (keys[EventKeyboard::KeyCode::KEY_W]) move1 = Point(0, 5);
		else if (keys[EventKeyboard::KeyCode::KEY_S]) move1 = Point(0, -5);
		else move1 = Point(0, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		keys[code] = false;
		if (keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW]) move2 = Point(0, -5);
		else if (keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW]) move2 = Point(-5, 0);
		else if (keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW]) move2 = Point(5, 0);
		else move2 = Point(0, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		keys[code] = false;
		if (keys[EventKeyboard::KeyCode::KEY_S]) move1 = Point(0, -5);
		else if (keys[EventKeyboard::KeyCode::KEY_A]) move1 = Point(-5, 0);
		else if (keys[EventKeyboard::KeyCode::KEY_D]) move1 = Point(5, 0);
		else move1 = Point(0, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		keys[code] = false;
		if (keys[EventKeyboard::KeyCode::KEY_UP_ARROW]) move2 = Point(0, 5);
		else if (keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW]) move2 = Point(-5, 0);
		else if (keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW]) move2 = Point(5, 0);
		else move2 = Point(0, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		keys[code] = false;
		if (keys[EventKeyboard::KeyCode::KEY_W]) move1 = Point(0, 5);
		else if (keys[EventKeyboard::KeyCode::KEY_A]) move1 = Point(-5, 0);
		else if (keys[EventKeyboard::KeyCode::KEY_D]) move1 = Point(5, 0);
		else move1 = Point(0, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_J:
		keys[code] = false;
		//angle = 0;
		//unschedule(schedule_selector(OperateLayer::updateAngle, 0.1f));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_K:
		keys[code] = false;
		//angle = 0;
		//unschedule(schedule_selector(OperateLayer::updateAngle, 0.1f));
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_8:
		keys[code] = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_9:
		keys[code] = false;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		if (mode == 1)
		{
			keys[code] = false;

			SimpleAudioEngine::getInstance()->playEffect("music/Fire1.mp3", false, 1.0f, 1.0f, 1.0f);

			unschedule(schedule_selector(OperateLayer::updatePercent1));
			hero1->pAttack();
			hero1->setPower(0);
		}

		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_0:
		if (mode == 1)
		{
			keys[code] = false;

			SimpleAudioEngine::getInstance()->playEffect("music/Fire3.mp3", false, 1.0f, 1.0f, 1.0f);

			unschedule(schedule_selector(OperateLayer::updatePercent2));
			hero2->pAttack();
			hero2->setPower(0);
		}

		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_I:
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_5:
		break;
	//case cocos2d::EventKeyboard::KeyCode::KEY_BACKSPACE:
	case cocos2d::EventKeyboard::KeyCode::KEY_P:
		if (mode == 1)
		{
			popPauseMenu();
		}
		
		break;
	default:
		keys[code] = false;
		break;
	}
}

void OperateLayer::update(float f)
{
	move1 = joy->getDirection() * 0.1 * joy->getVelocity();
	if (mode == 1)
	{
		hero1->pWalk(move1);
		hero2->pWalk(move2);
	}
	else if (mode == 2) amHero->pWalk(move1);
}

void OperateLayer::popPauseMenu()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto renderTexture = RenderTexture::create(origin.x + visibleSize.width, origin.y + visibleSize.height);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();
	auto pauseMenu = PauseMenu::createScene(renderTexture);
	auto curLayer = (PauseMenu*)pauseMenu->getChildren().at(2);
		// �����ú�Ľ������ѡ�˽���
	curLayer->backgroundVolume = SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
	curLayer->effectVolume = SimpleAudioEngine::getInstance()->getEffectsVolume();
	curLayer->curBgm = curBgm;
	//auto curLayer = (PauseMenu*)pauseMenu->getChildren().at(1);
	//curLayer->spr = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
	Director::getInstance()->pushScene(pauseMenu);
	}
