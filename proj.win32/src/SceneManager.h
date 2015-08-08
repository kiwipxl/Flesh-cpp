#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include <2d/CCLayer.h>
#include <2d/CCScene.h>

namespace cc = cocos2d;

class SceneManager : public cc::Layer {

	public:
		//auto-cleanup cocos macro
		CREATE_FUNC(SceneManager);

        static cc::Scene* create_scene();

        virtual bool init();
        
        virtual void menu_close(cc::Ref* r);
		virtual void update(float dt);

        static cc::PhysicsWorld* p_world;
        
        float time_since_startup = 0;
        cc::Size screen_size;
        cc::Vec2 screen_origin;
};

#endif