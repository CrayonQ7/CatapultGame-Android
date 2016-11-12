#include "Joystick.h"




Joystick * Joystick::create(float _radius, cocos2d::Sprite * _joystick, cocos2d::Sprite * _joystickBg)
{
	Joystick* pRet = new(std::nothrow) Joystick();
	if (pRet && pRet->init(_radius, _joystick, _joystickBg))
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

bool Joystick::init(float _radius, cocos2d::Sprite * _joystick, cocos2d::Sprite * _joystickBg)
{
	if (!Layer::init())
	{
		return false;
	}

	isActive = false;  // Ä¬ÈÏ²»¼¤»îÒ¡¸Ë
	center = Point(0, 0);
	current = center;
	radius = _radius;
	joystick = _joystick;
	joystickBg = _joystickBg;
	joystick->setPosition(center);
	joystickBg->setPosition(center);
	this->addChild(joystickBg);
	this->addChild(joystick);
	this->setVisible(false);  // ²»ÏÔÊ¾Ò¡¸Ë
	this->Active(); // ¼¤»îÒ¡¸Ë

	return true;
}

void Joystick::Active()
{
	if (!isActive)
	{
		isActive = true;
		schedule(schedule_selector(Joystick::updatePos));  // Ìí¼ÓË¢ÐÂº¯Êý
		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->onTouchBegan = CC_CALLBACK_2(Joystick::onTouchBegan, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(Joystick::onTouchMoved, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(Joystick::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	}
}

void Joystick::Inactive()
{
	if (isActive)
	{
		isActive = false;
		this->unschedule(schedule_selector(Joystick::updatePos));  // É¾³ýË¢ÐÂ
		_eventDispatcher->removeAllEventListeners();
	}
}

cocos2d::Point Joystick::getDirection()
{
	return ccpNormalize(ccpSub(current, center));
}

float Joystick::getVelocity()
{
	return ccpDistance(center, current);
}

void Joystick::updatePos(float dt)
{
	joystick->setPosition(ccpAdd(joystick->getPosition(), ccpMult(ccpSub(current, joystick->getPosition()), 0.5)));
}

bool Joystick::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (!isActive) return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	if (touch->getLocation().x > origin.x + visibleSize.width / 2) return false;
	auto touchPoint = this->convertToNodeSpace(touch->getLocation());

	center = touchPoint;
	current = center;
	joystick->setPosition(center);
	joystickBg->setPosition(center);
	this->setVisible(true);
	return true;
}

void Joystick::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	auto touchPoint = this->convertToNodeSpace(touch->getLocation());

	if (ccpDistance(touchPoint, center) > radius)
	{
		current = ccpAdd(center, ccpMult(ccpNormalize(ccpSub(touchPoint, center)), radius));
	}
	else
	{
		current = touchPoint;
	}
}

void Joystick::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	current = center;
	this->setVisible(false);
}
