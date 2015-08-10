#include "SceneManager.h"

#include <base/CCDirector.h>

#include "StateManager.h"

namespace root {

    using namespace cocos2d;

    Scene* SceneManager::create_scene() {
        auto scene = Scene::createWithPhysics();
        auto layer = new SceneManager();
        scene->addChild(layer);
        layer->init();
        layer->autorelease();

        return scene;
    }

    bool SceneManager::init() {
        if (!Layer::init()) return false;

        p_world = getScene()->getPhysicsWorld();
        screen_size = Director::getInstance()->getVisibleSize();
        screen_origin = Director::getInstance()->getVisibleOrigin();

        init_root(this);

        return true;
    }

    void SceneManager::update(float dt) {
        time_since_startup += dt;
        update_state(dt);
    }

    void SceneManager::onExit() {
        exit_root();
        Director::getInstance()->end();
        exit(0);
    }
};
