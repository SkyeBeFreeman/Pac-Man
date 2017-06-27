#include "GameScene.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <regex>
#include <string>
#include <sstream>
#include "RankScene.h"
#include "SelectScene.h"
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;

#pragma execution_character_set("UTF-8")

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

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	score = 0;

	auto quitButton = Button::create();
	quitButton->setTitleText("<");
	quitButton->setTitleFontSize(30);
	quitButton->setPosition(Size(quitButton->getSize().width, visibleSize.height - quitButton->getSize().height));
	quitButton->addClickEventListener(CC_CALLBACK_0(GameScene::quitEvent, this));
	this->addChild(quitButton, 2);

	addMap();
	addBean();
	addPlayer();
	addEnemy();
	addKeyboardListener();
	scheduleUpdate();
	monsterMove(0.1);

	return true;
}

// 提交成绩事件函数
void GameScene::submitEvent() {

	if (score > Global::maxscore) {
		HttpRequest* request = new HttpRequest();
		request->setUrl((string() + "http://" + Global::ip + ":11900/rank").c_str());
		request->setRequestType(HttpRequest::Type::POST);
		request->setResponseCallback(CC_CALLBACK_2(GameScene::onSubmitHttpCompleted, this));

		std::stringstream ss;
		ss << score;
		string temp;
		ss >> temp;

		// 写入POST请求数据
		string tmp = "username=" + Global::username + "&score=" + temp;
		const char* postData = tmp.c_str();
		request->setRequestData(postData, strlen(postData));
		request->setTag("POST test");

		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	} else {
		CCLOG("不够高啊");
	}
}

// 提交成绩请求结束回调函数
void GameScene::onSubmitHttpCompleted(HttpClient *sender, HttpResponse* response) {
	if (!response) {
		return;
	}
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	auto rankScene = RankScene::createScene();
	Director::getInstance()->replaceScene(rankScene);
}

// 退出点击事件
void GameScene::quitEvent() {
	auto selectScene = SelectScene::createScene();
	Director::getInstance()->replaceScene(selectScene);
}

