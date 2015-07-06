#include "State_Manager.h"

USING_NS_CC;

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

Label* label;

bool State_Manager::init() {
	if (!Layer::init()) return false;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
	label->setPosition(200, 200);
    this->addChild(label, 1);

    return true;
}

void State_Manager::update(float delta) {
	CCLOG("test");
	label->setPosition(Vec2(cos(delta), 20));
}

void State_Manager::menuCloseCallback(Ref* sender) {
	Director::getInstance()->end();
}
