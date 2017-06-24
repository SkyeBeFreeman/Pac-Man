#include "LoginScene.h"
#include "cocostudio/CocoStudio.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Global.h"
#include "GameScene.h"
#include <regex>
using std::to_string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace rapidjson;
USING_NS_CC;

using namespace cocostudio::timeline;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

#define database UserDefault::getInstance()

Scene* LoginScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LoginScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init() {
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	Size size = Director::getInstance()->getVisibleSize();
	visibleHeight = size.height;
	visibleWidth = size.width;

	textField = TextField::create("Player Name", "Arial", 30);
	textField->setPosition(Size(visibleWidth / 2, visibleHeight / 4 * 3));
	this->addChild(textField, 2);

	auto button = Button::create();
	button->setTitleText("Login");
	button->setTitleFontSize(30);
	button->setPosition(Size(visibleWidth / 2, visibleHeight / 2));
	button->addClickEventListener(CC_CALLBACK_0(LoginScene::loginEvent, this));
	this->addChild(button, 2);

	auto autoButton = Button::create();
	autoButton->setTitleText("Login Automatically");
	autoButton->setTitleFontSize(30);
	autoButton->setPosition(Size(visibleWidth / 2, visibleHeight / 2 - 50));
	autoButton->addClickEventListener(CC_CALLBACK_0(LoginScene::autoLoginEvent, this));
	this->addChild(autoButton, 2);

	return true;
}

// 登录事件函数
void LoginScene::loginEvent() {
	if (textField->getString() == "Player Name" || textField->getString() == "") {
		return;
	}

	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onLoginHttpCompleted, this));

	// 写入post请求数据
	string tmp = "username=" + textField->getString();
	const char* postData = tmp.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST test");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();

}

// 登录请求结束回调函数
void LoginScene::onLoginHttpCompleted(HttpClient *sender, HttpResponse* response) {
	if (!response) {
		return;
	}
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// 获取sessionID和username并保存在本地文件中
	Global::gameSessionId = Global::getSessionIdFromHeader(Global::toString(response->getResponseHeader()));
	database->setStringForKey("sessionID", Global::gameSessionId);
	database->setStringForKey("username", textField->getString());

	std::vector<char>* headBuffer = response->getResponseHeader();
	std::vector<char>* bodyBuffer = response->getResponseData();
	Global::loginHead = string(Global::toString(headBuffer));
	Global::loginBody = string(Global::toString(bodyBuffer));

	auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(gameScene);
}

// 自动登录事件函数
void LoginScene::autoLoginEvent() {
	if (database->getStringForKey("username") == "")
		return;

	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:8080/login");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onAutoLoginHttpCompleted, this));

	// 写入POST请求数据
	string tmp = "username=" + database->getStringForKey("username");
	const char* postData = tmp.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST test");

	// 写入POST请求头数据
	vector<string> headers;
	headers.push_back("Cookie: GAMESESSIONID=" + database->getStringForKey("sessionID"));
	request->setHeaders(headers);

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

// 自动登录请求结束回调函数
void LoginScene::onAutoLoginHttpCompleted(HttpClient *sender, HttpResponse* response) {
	if (!response) {
		return;
	}
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// 获取文件中的sessionID
	Global::gameSessionId = database->getStringForKey("sessionID");

	std::vector<char>* headBuffer = response->getResponseHeader();
	std::vector<char>* bodyBuffer = response->getResponseData();
	Global::loginHead = string(Global::toString(headBuffer));
	Global::loginBody = string(Global::toString(bodyBuffer));

	auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(gameScene);
}
