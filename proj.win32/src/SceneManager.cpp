#include "SceneManager.h"
#include "StateManager.h"

cc::Scene* SceneManager::create_scene() {
	//auto release objects
	auto scene = cc::Scene::create();
    auto layer = SceneManager::create();
	scene->addChild(layer);

    return scene;
}

bool SceneManager::init() {
    if (!Layer::init()) return false;
    
    screen_size = cc::Director::getInstance()->getVisibleSize();
    screen_origin = cc::Director::getInstance()->getVisibleOrigin();

    state::init(this);

    return true;
}

void SceneManager::update(float dt) {
    state::update(dt);
}

void SceneManager::menu_close(cc::Ref* r) {
    cc::Director::getInstance()->end();
}
