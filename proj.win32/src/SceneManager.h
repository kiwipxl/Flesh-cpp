#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include <2d/CCLayer.h>
#include <2d/CCScene.h>

namespace root {

    using namespace cocos2d;

    class SceneManager : public Layer {

        public:
            static Scene* create_scene();

            virtual bool init();

            virtual void menu_close(Ref* r);
            virtual void update(float dt);

            PhysicsWorld* p_world;

            float time_since_startup = 0;
            Size screen_size;
            Vec2 screen_origin;
    };
};

#endif