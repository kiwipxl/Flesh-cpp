#include "State_Manager.h"
#include <time.h>

using namespace cocos2d;

Scene* State_Manager::createScene() {
    // 'scene' is an autorelease object
	auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = State_Manager::create();

    // add layer as a child to scene
	scene->addChild(layer);

    // return the scene
    return scene;
}

bool State_Manager::init() {
	if (!Layer::init()) return false;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
	label->setPosition(200, 200);
    this->addChild(label, 1);

	scheduleUpdate();
	init_time = clock();

    return true;
}

void State_Manager::update(float dt) {
	time_since_startup = (clock() - init_time) / 1000.0f;
	label->setPosition(Vec2((cos(time_since_startup) * 40.0f) + 200, 200));
}

void State_Manager::menu_close(Ref* r) {
	Director::getInstance()->end();
}
