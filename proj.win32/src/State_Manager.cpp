#include "State_Manager.h"
#include <time.h>
#include "debug/Errors.h"
#include "network/MessageRecv.h"

#include <sstream>

#define SSTR(x) dynamic_cast<std::ostringstream&>((std::ostringstream() << std::dec << x )).str()

using namespace cocos2d;

LabelBMFont* State_Manager::label;

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
	label->setString("connecting...");
	addChild(label, 1);

	scheduleUpdate();
	init_time = clock();

	messagerecv::start();

    return true;
}

void State_Manager::update(float dt) {
	if (messagerecv::done_connecting) {
		if (messagerecv::connect_result == NO_ERROR) {
			label->setString("connected!");
			messagerecv::begin_receiving();
			messagerecv::done_connecting = false;
		}else {
			label->setString("an error occurred while trying to connect: " + SSTR(messagerecv::connect_result));
		}
	}

	time_since_startup = (clock() - init_time) / 1000.0f;
	label->setPosition(Vec2((cos(time_since_startup) * 40.0f) + 400, 200));

	/*for (int n = 0; n < 26; ++n) {
		Node* s;
		(s = label->getChildByTag(n))->setRotation(s->getRotation() + cos(s->getPosition().x));
	}

	for (int n = 0; n < 26; ++n) {
		Node* s;
		(s = label->getChildByTag(n))->setPosition((n * 25), s->getPosition().y);
	}*/
}

void State_Manager::menu_close(Ref* r) {
	Director::getInstance()->end();
}
