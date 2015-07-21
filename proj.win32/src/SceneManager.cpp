#include "SceneManager.h"
#include "StateManager.h"

using namespace cocos2d;

Scene* SceneManager::create_scene() {
	//auto release objects
	auto scene = Scene::create();
    auto layer = SceneManager::create();
	scene->addChild(layer);

    return scene;
}

bool SceneManager::init() {
    if (!Layer::init()) return false;

    state::init(this);

    return true;
}

void SceneManager::update(float dt) {
    state::update(dt);
}

void SceneManager::menu_close(Ref* r) {
	Director::getInstance()->end();
}
