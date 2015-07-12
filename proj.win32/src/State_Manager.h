#ifndef __STATE_MANAGER_SCENE_H__
#define __STATE_MANAGER_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class State_Manager : public Layer {

	public:
		//auto-cleanup cocos macro
		CREATE_FUNC(State_Manager);

		static Scene* createScene();

		virtual bool init();

		void menu_close(Ref* r);
		virtual void update(float dt);

		float time_since_startup;

	private:
		LabelBMFont* label;
		clock_t init_time;
};

#endif // __State_Manager_SCENE_H__
