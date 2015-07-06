#ifndef __STATE_MANAGER_SCENE_H__
#define __STATE_MANAGER_SCENE_H__

#include "cocos2d.h"

class State_Manager : public cocos2d::Layer {

	public:
		static cocos2d::Scene* createScene();

		virtual bool init();
    
		// a selector callback
		void menuCloseCallback(cocos2d::Ref* pSender);
		virtual void update(float delta);

		// implement the "static create()" method manually
		CREATE_FUNC(State_Manager);
};

#endif // __State_Manager_SCENE_H__
