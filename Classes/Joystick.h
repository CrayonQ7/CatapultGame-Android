#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"
USING_NS_CC;
class Joystick : public cocos2d::Layer
{
public:
	static Joystick* create(float _radius, cocos2d::Sprite* _joystick, cocos2d::Sprite* _joystickBg);
	virtual bool init(float _radius, cocos2d::Sprite* _joystick, cocos2d::Sprite* _joystickBg);
	void Active(); // ����ҡ��
	void Inactive(); // ȡ��ҡ��
	cocos2d::Point getDirection();  // ��ȡ����
	float getVelocity();  // ��ȡ�ٶ�
private:
	void updatePos(float dt);
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event *event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event *event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event *event);
	cocos2d::Point center;  // ҡ�����ĵ�����
	cocos2d::Point current;  // ��ǰҡ��λ��
	float radius; // ҡ�˰뾶
	cocos2d::Sprite* joystick; // ҡ�˿��Ƶ�
	cocos2d::Sprite* joystickBg; // ҡ�˱���
	bool isActive; // ��¼ҡ���Ƿ񱻼���
};


#endif // !__JOYSTICK_H__

