#include "State_Manager.h"
#include <time.h>
#include "network/MessageRecv.h"

using namespace cocos2d;

Scene* State_Manager::createScene() {
	//auto release objects
	auto scene = Scene::create();
    auto layer = State_Manager::create();
	scene->addChild(layer);

    return scene;
}

bool State_Manager::init() {
	if (!Layer::init()) return false;

    Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	label = LabelBMFont::create("", "fonts/lucida.fnt");
	label->setString("abcdefghijklmnopqrstuvwxyz");
	label->setPosition(200, 200);
	addChild(label, 1);

	scheduleUpdate();
	init_time = clock();

	messagerecv::start();

    return true;
}

void State_Manager::update(float dt) {
	time_since_startup = (clock() - init_time) / 1000.0f;
	label->setPosition(Vec2((cos(time_since_startup) * 40.0f) + 250, 200));

	for (int n = 0; n < 26; ++n) {
		Node* s;
		(s = label->getChildByTag(n))->setRotation(s->getRotation() + cos(s->getPosition().x));
	}

	for (int n = 0; n < 26; ++n) {
		Node* s;
		(s = label->getChildByTag(n))->setPosition((n * 25), s->getPosition().y);
	}
}

void State_Manager::menu_close(Ref* r) {
	Director::getInstance()->end();
}
