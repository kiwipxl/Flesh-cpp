#include "physics/Physics.h"

#include <base/CCEventDispatcher.h>
#include <physics/CCPhysicsWorld.h>

#include "debug/Logger.h"
#include "StateManager.h"

BEGIN_PHYSICS_NS

using namespace cocos2d;

//private
struct ContactData {

    ContactData(void* r) : ref(r) { }

    void* ref;
    RunContactCallback run_call;
    PostContactCallback post_call;
    LeaveContactCallback leave_call;
};

EventListenerPhysicsContact* contact_listener;
std::vector<ContactData*> run_contact_callbacks;
std::vector<ContactData*> post_contact_callbacks;
std::vector<ContactData*> leave_contact_callbacks;

time_t start_log_time;
float physics_timing = 0;
const float TIMESTEP = 1.0f / 60.0f;

bool on_contact_pre_solve(PhysicsContact& contact, PhysicsContactPreSolve& pre_solve) {
    bool do_collide = false;
    for (int n = 0; n < run_contact_callbacks.size(); ++n) {
        if (run_contact_callbacks[n]->run_call(contact)) do_collide = true;
    }
    return do_collide;
}

void on_contact_post_solve(PhysicsContact& contact, const PhysicsContactPostSolve& post_solve) {
    for (int n = 0; n < post_contact_callbacks.size(); ++n) {
        post_contact_callbacks[n]->post_call(contact, post_solve);
    }
}

void on_contact_separate(PhysicsContact& contact) {
    for (int n = 0; n < post_contact_callbacks.size(); ++n) {
        leave_contact_callbacks[n]->leave_call(contact);
    }
}

void remove_contact_listener(std::vector<ContactData*>& v, void* callback, int offset) {
    for (int n = 0; n < v.size(); ++n) {
        if (callback == v[n] + offset) {
            v.erase(v.begin() + n, v.begin() + n + 1);
            --n;
        }
    }
}

void remove_contact_listener(std::vector<ContactData*>& v, void* ref) {
    for (int n = 0; n < v.size(); ++n) {
        if (ref == v[n]->ref) {
            v.erase(v.begin() + n, v.begin() + n + 1);
            --n;
        }
    }
}

//public
void add_on_contact_run(RunContactCallback callback, void* ref) {
    auto data = new ContactData(ref);
    data->run_call = callback;
    run_contact_callbacks.push_back(data);
}

void add_on_post_contact(PostContactCallback callback, void* ref) {
    auto data = new ContactData(ref);
    data->post_call = callback;
    post_contact_callbacks.push_back(data);
}

void add_on_contact_leave(LeaveContactCallback callback, void* ref) {
    auto data = new ContactData(ref);
    data->leave_call = callback;
    leave_contact_callbacks.push_back(data);
}

//remove calls begin
void remove_on_contact_run(RunContactCallback callback) {
    remove_contact_listener(run_contact_callbacks, &callback, offsetof(ContactData, run_call));
}
void remove_on_contact_run(void* ref) {
    remove_contact_listener(run_contact_callbacks, ref);
}

void remove_on_post_contact(PostContactCallback callback) {
    remove_contact_listener(run_contact_callbacks, &callback, offsetof(ContactData, post_call));
}
void remove_on_post_contact(void* ref) {
    remove_contact_listener(run_contact_callbacks, ref);
}

void remove_on_contact_leave(LeaveContactCallback callback) {
    remove_contact_listener(run_contact_callbacks, &callback, offsetof(ContactData, leave_call));
}
void remove_on_contact_leave(void* ref) {
    remove_contact_listener(run_contact_callbacks, ref);
}

void remove_all_contact_listeners(void* ref) {
    remove_on_contact_run(ref);
    remove_on_post_contact(ref);
    remove_on_contact_leave(ref);
}
//remove calls end

void init() {
    time(&start_log_time);
    contact_listener = EventListenerPhysicsContact::create();
    contact_listener->onContactPreSolve = on_contact_pre_solve;
    contact_listener->onContactPostSolve = on_contact_post_solve;
    contact_listener->onContactSeperate = on_contact_separate;
    root::scene->getEventDispatcher()->addEventListenerWithFixedPriority(contact_listener, 10);
}

void start() {
    root::scene->p_world->setAutoStep(false);
    root::scene->p_world->setGravity(Vec2(0, -980.0f));
}

void update() {
    root::scene->p_world->step(TIMESTEP);
    /*physics_timing += delta_time;
    while (physics_timing >= TIMESTEP) {
    scene->p_world->step(TIMESTEP);
    physics_timing -= TIMESTEP;
    }*/

    //after a set amount of time, log num callbacks for debugging purposes
    time_t temp_time;
    if (time(&temp_time) - start_log_time >= 5.0f) {
        log_info << "contact pre_solve callbacks: " << run_contact_callbacks.size() <<
            ", post_solve: " << post_contact_callbacks.size() <<
            ", separate: " << leave_contact_callbacks.size();
        start_log_time = temp_time;
    }
}

END_PHYSICS_NS
