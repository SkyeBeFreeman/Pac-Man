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

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	enum MOVE { UP, DOWN, LEFT, RIGHT, NO_DIRECTION };
	
	// add player
	void addMap();

	// add beans
	void addBean();

	// add player
	void addPlayer();
	
	// add enemies
	void addEnemy();

	// update to new position of enemies
	void addKeyboardListener();
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	void update(float f) override;
	void monsterMove(float f);
	void redMove(float f);
	void orangeMove(float f);
	void pinkMove(float f);
	void blueMove(float f);
	bool collide(Sprite *s1, Sprite *s2);
	bool collide(Sprite *s1, TMXObjectGroup *w);
	void becomeSuper();
	void becomenormal(float f);
	// a selector callback
	void menuCloseCallback(cocos2d::Ref *pSender);

	// 跳转到结算页面
	void toEndScene(cocos2d::Ref *pSender, bool isWin);

	void submitEvent();

	void onSubmitHttpCompleted(HttpClient *sender, HttpResponse* response);

	void quitEvent();

private:
	int p_x;
	int p_y;
	int width;
	int height;
	std::vector<Sprite*> beans;
	TMXTiledMap *map;
	TMXObjectGroup *wall;
	cocos2d::Sprite *player;
	Vec2 origin;
	Size visibleSize;
	MOVE move = MOVE::NO_DIRECTION;
	Sprite *enemyBlue, *enemyRed, *enemyOrange, *enemyPink, *bigBean1, *bigBean2, *bigBean3, *bigBean4;
	int o_x, o_y, num;
	bool t_f, issupered = false;
	EventListenerKeyboard * keyboardListener;
	TextField * score_field;
};

