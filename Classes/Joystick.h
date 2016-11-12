#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"
USING_NS_CC;
class Joystick : public cocos2d::Layer
{
public:
	static Joystick* create(float _radius, cocos2d::Sprite* _joystick, cocos2d::Sprite* _joystickBg);
	virtual bool init(float _radius, cocos2d::Sprite* _joystick, cocos2d::Sprite* _joystickBg);
	void Active(); // 启动摇杆
	void Inactive(); // 取消摇杆
	cocos2d::Point getDirection();  // 获取方向
	float getVelocity();  // 获取速度
private:
	void updatePos(float dt);
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event *event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event *event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event *event);
	cocos2d::Point center;  // 摇杆中心点坐标
	cocos2d::Point current;  // 当前摇杆位置
	float radius; // 摇杆半径
	cocos2d::Sprite* joystick; // 摇杆控制点
	cocos2d::Sprite* joystickBg; // 摇杆背景
	bool isActive; // 记录摇杆是否被激活
};


#endif // !__JOYSTICK_H__

