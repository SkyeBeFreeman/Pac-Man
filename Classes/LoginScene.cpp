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

	usernameField = TextField::create("Player Name", "Arial", 30);
	usernameField->setPosition(Size(visibleWidth / 2, visibleHeight / 6 * 5));
	this->addChild(usernameField, 2);

	passwordField = TextField::create("Player Password", "Arial", 30);
	passwordField->setPosition(Size(visibleWidth / 2, visibleHeight / 6 * 4));
	this->addChild(passwordField, 2);

	errorField = TextField::create("", "Arial", 20);
	errorField->setPosition(Size(visibleWidth / 2, visibleHeight / 6 * 3));
	this->addChild(errorField, 2);

	auto registButton = Button::create();
	registButton->setTitleText("Regist");
	registButton->setTitleFontSize(30);
	registButton->setPosition(Size(visibleWidth / 5 * 2, visibleHeight / 6 * 2));
	registButton->addClickEventListener(CC_CALLBACK_0(LoginScene::registEvent, this));
	this->addChild(registButton, 2);

	auto loginButton = Button::create();
	loginButton->setTitleText("Login");
	loginButton->setTitleFontSize(30);
	loginButton->setPosition(Size(visibleWidth / 5 * 3, visibleHeight / 6 * 2));
	loginButton->addClickEventListener(CC_CALLBACK_0(LoginScene::loginEvent, this));
	this->addChild(loginButton, 2);

	auto autoLoginButton = Button::create();
	autoLoginButton->setTitleText("Login Automatically");
	autoLoginButton->setTitleFontSize(30);
	autoLoginButton->setPosition(Size(visibleWidth / 2, visibleHeight / 6));
	autoLoginButton->addClickEventListener(CC_CALLBACK_0(LoginScene::autoLoginEvent, this));
	this->addChild(autoLoginButton, 2);

	return true;
}

// 注册事件函数
void LoginScene::registEvent() {
	if (usernameField->getString() == "Player Name" || usernameField->getString() == ""
		|| passwordField->getString() == "Player Password" || passwordField->getString() == "") {
		return;
	}

	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:11900/regist");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onRegistHttpCompleted, this));

	// 写入post请求数据
	string tmp = "username=" + usernameField->getString() + "&password=" + passwordField->getString();
	const char* postData = tmp.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST test");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();

	usernameField->setString("");
	passwordField->setString("");

}

// 注册请求结束回调函数
void LoginScene::onRegistHttpCompleted(HttpClient* sender, HttpResponse* response) {
	if (!response) {
		return;
	}

	std::vector<char>* headBuffer = response->getResponseHeader();
	std::vector<char>* bodyBuffer = response->getResponseData();
	Global::loginHead = string(Global::toString(headBuffer));
	Global::loginBody = string(Global::toString(bodyBuffer));

	errorField->setString(Global::loginBody);
}



// 登录事件函数
void LoginScene::loginEvent() {
	if (usernameField->getString() == "Player Name" || usernameField->getString() == ""
		|| passwordField->getString() == "Player Password" || passwordField->getString() == "") {
		return;
	}

	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:11900/login");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onLoginHttpCompleted, this));

	// 写入post请求数据
	string tmp = "username=" + usernameField->getString() + "&password=" + passwordField->getString();
	const char* postData = tmp.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST test");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();

	usernameField->setString("");
	passwordField->setString("");
}

// 登录请求结束回调函数
void LoginScene::onLoginHttpCompleted(HttpClient *sender, HttpResponse* response) {
	if (!response) {
		return;
	}

	std::vector<char>* headBuffer = response->getResponseHeader();
	std::vector<char>* bodyBuffer = response->getResponseData();
	Global::loginHead = string(Global::toString(headBuffer));
	Global::loginBody = string(Global::toString(bodyBuffer));

	if (!response->isSucceed()) {
		errorField->setString(Global::loginBody);
		return;
	}

	// 获取sessionID和username并保存在本地文件中
	Global::gameSessionId = Global::getSessionIdFromHeader(Global::toString(response->getResponseHeader()));
	database->setStringForKey("sessionID", Global::gameSessionId);
	database->setStringForKey("username", usernameField->getString());
	database->setStringForKey("password", passwordField->getString());

	auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(gameScene);
}

// 自动登录事件函数
void LoginScene::autoLoginEvent() {
	if (database->getStringForKey("username") == "")
		return;

	HttpRequest* request = new HttpRequest();
	request->setUrl("http://localhost:11900/login");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(LoginScene::onAutoLoginHttpCompleted, this));

	// 写入POST请求数据
	string tmp = "username=" + database->getStringForKey("username") + "&password=" + database->getStringForKey("password");
	const char* postData = tmp.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("POST test");

	// 写入POST请求头数据
	vector<string> headers;
	headers.push_back("Cookie: SESSION=" + database->getStringForKey("sessionID"));
	request->setHeaders(headers);

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();

	usernameField->setString("");
	passwordField->setString("");
}

// 自动登录请求结束回调函数
void LoginScene::onAutoLoginHttpCompleted(HttpClient *sender, HttpResponse* response) {
	if (!response) {
		return;
	}
	
	std::vector<char>* headBuffer = response->getResponseHeader();
	std::vector<char>* bodyBuffer = response->getResponseData();
	Global::loginHead = string(Global::toString(headBuffer));
	Global::loginBody = string(Global::toString(bodyBuffer));

	if (!response->isSucceed()) {
		errorField->setString(Global::loginBody);
		return;
	}

	// 获取文件中的sessionID
	Global::gameSessionId = database->getStringForKey("sessionID");

	auto gameScene = GameScene::createScene();
	Director::getInstance()->replaceScene(gameScene);
}
