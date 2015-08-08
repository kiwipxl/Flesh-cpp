#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include <2d/CCLayer.h>
#include <2d/CCScene.h>

namespace root {

    namespace cc = cocos2d;

    class SceneManager : public cc::Layer {

        public:
            static cc::Scene* create_scene();

            virtual bool init();

            virtual void menu_close(Ref* r);
            virtual void update(float dt);

            cc::PhysicsWorld* p_world;

            float time_since_startup = 0;
            cc::Size screen_size;
            cc::Vec2 screen_origin;
    };
};

#endif