#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "cocos2d.h"

using namespace cocos2d;

class SceneManager : public Layer {

	public:
		//auto-cleanup cocos macro
		CREATE_FUNC(SceneManager);

        static Scene* create_scene();

        virtual bool init();
        
        virtual void menu_close(Ref* r);
		virtual void update(float dt);

        float time_since_startup = 0;
};

#endif