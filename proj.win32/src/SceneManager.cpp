#include "SceneManager.h"

#include <base/CCDirector.h>

#include "StateManager.h"

namespace root {

    Scene* SceneManager::create_scene() {
        //auto release objects
        auto scene = Scene::createWithPhysics();
        scene->retain();
        auto layer = SceneManager::create();
        scene->addChild(layer);

        return scene;
    }

    bool SceneManager::init() {
        if (!Layer::init()) return false;

        Scene* s = getScene();
        p_world = getScene()->getPhysicsWorld();
        screen_size = Director::getInstance()->getVisibleSize();
        screen_origin = Director::getInstance()->getVisibleOrigin();

        init_root(this);

        return true;
    }

    void SceneManager::update(float dt) {
        time_since_startup += dt;
        update(dt);
    }

    void SceneManager::menu_close(Ref* r) {
        Director::getInstance()->end();
    }
};
