#include "physics/Physics.h"

#include <base/CCEventDispatcher.h>

#include "debug/Logger.h"
#include "StateManager.h"

BEGIN_PHYSICS_NS

using namespace cocos2d;

//private
struct RunContact {

    RunContact(RunContactCallback f, void* r) : func(f), ref(r) { }

    void* ref;
    RunContactCallback func;
};

std::vector<RunContact*> run_contact_callbacks;
time_t start_log_time;

bool on_contact_pre_solve(PhysicsContact& contact, PhysicsContactPreSolve& pre_contact) {
    //after a set amount of time, log pre solve callbacks for debugging purposes
    time_t temp_time;
    if (time(&temp_time) - start_log_time >= 2.0f) {
        log_info << "contact pre_solve callbacks: " << run_contact_callbacks.size();
        start_log_time = temp_time;
    }

    bool do_collide = false;
    for (int n = 0; n < run_contact_callbacks.size(); ++n) {
        if (run_contact_callbacks[n]->func(contact)) do_collide = true;
    }
    return do_collide;
}

void add_on_contact_run(RunContactCallback callback, void* ref) {
    run_contact_callbacks.push_back(new RunContact(callback, ref));
}

void remove_on_contact_run(RunContactCallback callback) {
    for (int n = 0; n < run_contact_callbacks.size(); ++n) {
        if (&callback == &run_contact_callbacks[n]->func) {
            run_contact_callbacks.erase(run_contact_callbacks.begin() + n, run_contact_callbacks.begin() + n + 1);
            --n;
        }
    }
}

void remove_on_contact_run(void* ref) {
    for (int n = 0; n < run_contact_callbacks.size(); ++n) {
        if (ref == run_contact_callbacks[n]->ref) {
            run_contact_callbacks.erase(run_contact_callbacks.begin() + n, run_contact_callbacks.begin() + n + 1);
            --n;
        }
    }
}

//public
EventListenerPhysicsContact* contact_listener;

void init() {
    time(&start_log_time);
    contact_listener = EventListenerPhysicsContact::create();
    contact_listener->onContactPreSolve = on_contact_pre_solve;
    root::scene->getEventDispatcher()->addEventListenerWithFixedPriority(contact_listener, 10);
}

END_PHYSICS_NS
