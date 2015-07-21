#include "StateManager.h"
#include <sstream>
#include "debug/Errors.h"
#include "network/MessageRecv.h"

using state::State;

#define SSTR(x) dynamic_cast<std::ostringstream&>((std::ostringstream() << std::dec << x)).str()

SceneManager* state::scene;

State state::s = state::INIT_SCENE;
LabelBMFont* state::label;
float state::time_since_startup = 0;

void state::switch_state(State new_state) {

}

void state::init(SceneManager* scene_ref) {
    scene = scene_ref;

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    label = LabelBMFont::create("", "fonts/lucida.fnt");
    label->setString("connecting...");
    scene->addChild(label, 1);

    scene->scheduleUpdate();

    messagerecv::connect_to_server();
}

void state::update(float dt) {
    if (messagerecv::done_connecting) {
        if (messagerecv::connect_result == NO_ERROR) {
            label->setString("connected!");
            messagerecv::done_connecting = false;
        }
        else {
            label->setString("an error occurred while trying to connect: " + SSTR(messagerecv::connect_result));
        }
    }

    time_since_startup += dt;
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