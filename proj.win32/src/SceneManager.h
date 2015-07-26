#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "cocos2d.h"

namespace cc = cocos2d;

class SceneManager : public cocos2d::Layer {

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