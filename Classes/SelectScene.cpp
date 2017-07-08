#include "SelectScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#include "LoginScene.h"
#include "RankScene.h"
#include "GameScene.h"
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

#pragma execution_character_set("UTF-8")

USING_NS_CC;

cocos2d::Scene* SelectScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SelectScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool SelectScene::init() {
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	visibleHeight = visibleSize.height;
	visibleWidth = visibleSize.width;

	auto bg = Sprite::create("t.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	auto gameButton = Button::create();
	gameButton->setTitleText("Start Game");
	gameButton->setTitleFontSize(30);
	gameButton->setTitleFontName("fonts/Marker Felt.TTF");
	gameButton->setPosition(Size(visibleWidth / 2, visibleHeight / 5 * 3));
	gameButton->addClickEventListener(CC_CALLBACK_0(SelectScene::toGameScene, this));
	this->addChild(gameButton, 2);

	auto rankButton = Button::create();
	rankButton->setTitleText("Rank");
	rankButton->setTitleFontSize(30);
	rankButton->setTitleFontName("fonts/Marker Felt.TTF");
	rankButton->setPosition(Size(visibleWidth / 2, visibleHeight / 5 * 2));
	rankButton->addClickEventListener(CC_CALLBACK_0(SelectScene::toRankScene, this));
	this->addChild(rankButton, 2);

	auto logoutButton = Button::create();
	logoutButton->setTitleText("Logout");
	logoutButton->setTitleFontSize(30);
	logoutButton->setTitleFontName("fonts/Marker Felt.TTF");
	logoutButton->setPosition(Size(visibleWidth / 2, visibleHeight / 5));
	logoutButton->addClickEventListener(CC_CALLBACK_0(SelectScene::logout, this));
	this->addChild(logoutButton, 2);

	return true;
}

// 跳转到游戏界面
void SelectScene::toGameScene() {
	auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(gameScene);
}

// 跳转到排名界面
void SelectScene::toRankScene() {
	auto rankScene = RankScene::createScene();
	Director::getInstance()->replaceScene(rankScene);
}

// 登出事件
void SelectScene::logout() {
	auto loginScene = LoginScene::createScene();
	Director::getInstance()->replaceScene(loginScene);
}
