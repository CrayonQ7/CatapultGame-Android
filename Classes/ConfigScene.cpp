#include "ConfigScene.h"
#include "VersusModeScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace ui;
using namespace CocosDenshion;

USING_NS_CC;
int ConfigScene::curBgm = 0;
Scene* ConfigScene::createScene(RenderTexture* sqr) {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	auto spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
	spr->setColor(cocos2d::Color3B::GRAY);  //图片变灰色
	spr->setAnchorPoint(ccp(0, 0));
	spr->setFlippedY(true);
	scene->addChild(spr);
	// 'layer' is an autorelease object
	auto layer = ConfigScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ConfigScene::init()
{
	if (!Layer::init()) {
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();




	// 标题
	auto label = Label::createWithTTF("Config", "fonts/Marker Felt.ttf", 40);
	label->setColor(Color3B::BLACK);
	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height*2));
	this->addChild(label, 1);

	// 选择背景音乐
	auto musicSelectLabel = Label::createWithTTF("BackgroundMusic:", "fonts/Marker Felt.ttf", 40);
	musicSelectLabel->setColor(Color3B::BLACK);
	musicSelectLabel->setPosition(Vec2(origin.x + musicSelectLabel->getContentSize().width/2+visibleSize.width/4,
		origin.y + visibleSize.height - musicSelectLabel->getContentSize().height * 3.5));
	this->addChild(musicSelectLabel, 1);

	    // 显示背景音乐
	musicLabel = Label::createWithTTF(loadbgm[curBgm], "fonts/1.ttf", 32);
	//musicLabel->setScale(2);
	musicLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 + musicLabel->getContentSize().width*2-30,
		musicSelectLabel->getPositionY()));
	this->addChild(musicLabel, 1);

	// 调节BGM音量
	auto musicControlLabel = Label::createWithTTF("MusicVolume:", "fonts/Marker Felt.ttf", 40);
	musicControlLabel->setColor(Color3B::BLACK);
	musicControlLabel->setPosition(Vec2(origin.x + musicControlLabel->getContentSize().width/2+ visibleSize.width / 4,
		origin.y + visibleSize.height - musicControlLabel->getContentSize().height * 5.5));
	this->addChild(musicControlLabel, 1);

	// 调节音效音量
	auto effectControlLabel = Label::createWithTTF("EffectsVolume:", "fonts/Marker Felt.ttf", 40);
	effectControlLabel->setColor(Color3B::BLACK);
	effectControlLabel->setPosition(Vec2(origin.x + effectControlLabel->getContentSize().width/2+ visibleSize.width / 4,
		origin.y + visibleSize.height - effectControlLabel->getContentSize().height * 7.5));
	this->addChild(effectControlLabel, 1);

	testItem = MenuItemImage::create("icons/testNormal.png", "icons/testSelected.png", [&](Ref* pSender)
	{
		// 播放点击音效
		SimpleAudioEngine::getInstance()->setEffectsVolume(effectVolume);
		SimpleAudioEngine::getInstance()->playEffect("music/button.mp3", false, 1.0f, 1.0f, 1.0f);
	});
	testItem->setPosition(musicLabel->getPositionX() + musicLabel->getContentSize().width / 2 + 30, effectControlLabel->getPositionY());

	// 返回按钮
	auto backItem = MenuItemImage::create("icons/BackNormal.png", "icons/BackSelected.png", CC_CALLBACK_1(ConfigScene::sceneBackCallback, this));
	backItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 3 + 20));

	auto menu = Menu::create(backItem, testItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	// 背景图片
	//auto background = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("background.png"));
	auto background = Sprite::create("plist/background.png");
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	auto scaleX = background->getTextureRect().getMaxX();
	auto scaleY = background->getTextureRect().getMaxY();
    background->setScaleX(visibleSize.width / scaleX);
	background->setScaleY(visibleSize.height / scaleY);
	this->addChild(background, 0);
	return true;
}

void ConfigScene::onEnter() {
	Layer::onEnter();

	// 背景音乐音量滑动条
	bSlider = Slider::create();
	bSlider->loadBarTexture("icons/SliderTrack.png");
	bSlider->loadSlidBallTextures("icons/SliderThumb.png", "icons/SliderThumb.png", "");
	bSlider->loadProgressBarTexture("icons/SliderBar.png");
	bSlider->setScale9Enabled(true);
	bSlider->setCapInsets(Rect(0, 0, 0, 0));
	bSlider->setContentSize(Size(400, 30));
	bSlider->setPosition(Vec2(origin.x + visibleSize.width / 1.7f,
		(musicLabel->getPositionY()+ testItem->getPositionY())/2));
	bSlider->addEventListener(CC_CALLBACK_2(ConfigScene::backgroundVolumeSliderEvent, this));
	bSlider->setPercent((int)(backgroundVolume * 100));
	bSlider->setScale(0.5);
	this->addChild(bSlider, 1);

	// 音效音量滑动条
	eSlider = Slider::create();
	eSlider->loadBarTexture("icons/SliderTrack.png");
	eSlider->loadSlidBallTextures("icons/SliderThumb.png", "icons/SliderThumb.png", "");
	eSlider->loadProgressBarTexture("icons/SliderBar.png");
	eSlider->setScale9Enabled(true);
	eSlider->setCapInsets(Rect(0, 0, 0, 0));
	eSlider->setContentSize(Size(400, 30));
	eSlider->setPosition(Vec2(origin.x + visibleSize.width / 1.7f,
		testItem->getPositionY()));
	eSlider->addEventListener(CC_CALLBACK_2(ConfigScene::effectVolumeSliderEvent, this));
	eSlider->setPercent((int)(effectVolume * 100));
	eSlider->setScale(0.5);
	this->addChild(eSlider, 1);


	// 左选按钮
	m_leftSelectItem = MenuItemImage::create("icons/LeftNormal.png", "icons/LeftSelected.png", "icons/LeftDisabled.png",
		CC_CALLBACK_1(ConfigScene::m_leftSelectCallback, this));
	m_leftSelectItem->setPosition(Vec2(musicLabel->getPositionX() - musicLabel->getContentSize().width / 2-30, musicLabel->getPositionY()));
	m_leftSelectItem->setScale(0.6f);
	//if (curBgm == 0) m_leftSelectItem->setEnabled(false);

	// 右选按钮
	m_rightSelectItem = MenuItemImage::create("icons/RightNormal.png", "icons/RightSelected.png", "icons/RightDisabled.png",
		CC_CALLBACK_1(ConfigScene::m_rightSelectCallback, this));
	m_rightSelectItem->setPosition(Vec2(musicLabel->getPositionX() + musicLabel->getContentSize().width/2+30, musicLabel->getPositionY()));
	m_rightSelectItem->setScale(0.6f);

	auto menuItem = Menu::create(m_leftSelectItem, m_rightSelectItem, NULL);
	menuItem->setPosition(Vec2::ZERO);
	this->addChild(menuItem, 1);
}

void ConfigScene::onEnterTransitionDidFinish() {
	Layer::onEnterTransitionDidFinish();
	// 播放初始选中的BGM
	char file[20];
	sprintf(file, "music/BGM_%d.mp3", curBgm + 1);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(file, true);
}

void ConfigScene::m_leftSelectCallback(Ref* pSender) {
	char file[20];
	if (curBgm > 0)
	{
		curBgm -= 1;
		sprintf(file, "music/BGM_%d.mp3", curBgm + 1);
	}
	else if (curBgm == 0)
	{
		curBgm = musics - 1;
		sprintf(file, "music/BGM_%d.mp3", curBgm + 1);
	}
	musicLabel->setString(loadbgm[curBgm]);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(file, true);
	backgroundVolume = 0.5f;
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(backgroundVolume);
	bSlider->setPercent((int)(backgroundVolume * 100));
}
void ConfigScene::m_rightSelectCallback(Ref* pSender)
{
	char file[20];
	if (curBgm < musics - 1)
	{
		curBgm += 1;
		sprintf(file, "music/BGM_%d.mp3", curBgm + 1);
	}
	if (curBgm == musics - 1)
	{
		curBgm = 0;
		sprintf(file, "music/BGM_%d.mp3", curBgm + 1);
	}
	musicLabel->setString(loadbgm[curBgm]);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(file, true);
	backgroundVolume = 0.5f;
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(backgroundVolume);
	bSlider->setPercent((int)(backgroundVolume * 100));
}

void ConfigScene::sceneBackCallback(Ref* pSender)
{
	//auto menuScene = VersusModeScene::createScene();
	//auto curLayer = (VersusModeScene*)menuScene->getChildren().at(1);
	//// 将设置后的结果传回选人界面
	//curLayer->backgroundVolume = backgroundVolume;
	//curLayer->effectVolume = effectVolume;
	//curLayer->curBgm = curBgm;
	//Director::getInstance()->replaceScene(CCTransitionSlideInB::create(0.5f, menuScene));
	VersusModeScene::backgroundVolume = backgroundVolume;
	VersusModeScene::effectVolume = effectVolume;
	VersusModeScene::curBgm = curBgm;
	Director::getInstance()->popScene();
}

void ConfigScene::backgroundVolumeSliderEvent(Ref* pSender, Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(pSender);
		int percent = slider->getPercent();
		backgroundVolume = (float)percent / 100.0f;
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(backgroundVolume);
	}
}

void ConfigScene::effectVolumeSliderEvent(Ref* pSender, Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(pSender);
		int percent = slider->getPercent();
		effectVolume = (float)percent / 100.0f;
		SimpleAudioEngine::getInstance()->setEffectsVolume(effectVolume);
	}
}
