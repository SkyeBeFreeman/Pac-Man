#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include "Global.h"
using namespace cocos2d;
using namespace cocos2d::ui;

#include "network/HttpClient.h"
using namespace cocos2d::network;

using std::string;

class EndScene : public cocos2d::Layer {
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(EndScene);

	void quitEvent();

private:
	Vec2 origin;
	Size visibleSize;
	float visibleHeight;
	float visibleWidth;
	TextField * username_field;
	TextField * score_field;
	TextField * ranking_field;
	TextField * newranking_field;
};

