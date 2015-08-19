#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <functional>

#include <physics/CCPhysicsContact.h>
#include <physics/CCPhysicsBody.h>

#include "physics/PhysicsDefines.h"

BEGIN_PHYSICS_NS

namespace cc = cocos2d;

extern cc::EventListenerPhysicsContact* contact_listener;

typedef std::function<bool(cc::PhysicsContact&)> RunContactCallback;
typedef std::function<void(cc::PhysicsContact&, const cc::PhysicsContactPostSolve&)> PostContactCallback;
typedef std::function<void(cc::PhysicsContact&)> LeaveContactCallback;

extern void init();
extern void start();
extern void update();
extern void add_on_contact_run(RunContactCallback callback, void* ref);
extern void add_on_post_contact(PostContactCallback callback, void* ref);
extern void add_on_contact_leave(LeaveContactCallback callback, void* ref);
//extern void add_on_contact_once(RunContactCallback callback);

extern void remove_on_contact_run(RunContactCallback callback);
extern void remove_on_contact_run(void* ref);

extern void remove_on_post_contact(PostContactCallback callback);
extern void remove_on_post_contact(void* ref);

extern void remove_on_contact_leave(LeaveContactCallback callback);
extern void remove_on_contact_leave(void* ref);

extern void remove_all_contact_listeners(void* ref);

END_PHYSICS_NS

#endif