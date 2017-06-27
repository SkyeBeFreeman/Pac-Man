#include "EndScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#include <string>
#include <sstream>
#include "SelectScene.h"
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

#pragma execution_character_set("UTF-8")

USING_NS_CC;

cocos2d::Scene* EndScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = EndScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool EndScene::init() {
	if (!Layer::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	visibleHeight = visibleSize.height;
	visibleWidth = visibleSize.width;

	auto label = Label::createWithTTF(Global::status, "fonts/Marker Felt.TTF", 40);

	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - 2 * label->getContentSize().height));

	this->addChild(label, 1);

	username_field = TextField::create(Global::username, "fonts/Marker Felt.TTF", 30);
	username_field->setPosition(Size(visibleWidth / 2, visibleHeight / 2));
	this->addChild(username_field, 2);

	std::stringstream ss;
	ss << Global::score;
	string temp;
	ss >> temp;

	score_field = TextField::create(temp, "fonts/Marker Felt.TTF", 30);
	score_field->setPosition(Size(visibleWidth / 2, visibleHeight / 5 * 2));
	this->addChild(score_field, 2);

	auto quitButton = Button::create();
	quitButton->setTitleText("<");
	quitButton->setTitleFontSize(30);
	quitButton->setPosition(Size(quitButton->getSize().width, visibleHeight - quitButton->getSize().height));
	quitButton->addClickEventListener(CC_CALLBACK_0(EndScene::quitEvent, this));
	this->addChild(quitButton, 2);

	return true;
}

// 退出点击事件
void EndScene::quitEvent() {
	auto selectScene = SelectScene::createScene();
	Director::getInstance()->replaceScene(selectScene);
}

