#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

USING_NS_CC;

cocos2d::Scene* GameScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	visibleHeight = visibleSize.height;
	visibleWidth = visibleSize.width;

	head_field = TextField::create("", "Arial", 15);
	head_field->setPosition(Size(visibleWidth / 4, visibleHeight / 4 * 2));
	head_field->setString(Global::loginHead);
	this->addChild(head_field, 2);

	body_field = TextField::create("", "Arial", 20);
	body_field->setPosition(Size(visibleWidth / 4 * 3, visibleHeight / 4 * 2));
	body_field->setString(Global::loginBody);
	this->addChild(body_field, 2);

	return true;
}
