#ifndef _ENTITY_SCHEDULER_H_
#define _ENTITY_SCHEDULER_H_

/*
class used to inherit from that handles whether an entity is "scheduled" (or planned) to be removed.
this is generally used when an entity wants to destroy itself and a for loop must handle this.

for example:
for (auto& entity : vec) {
    entity->update();
    if (entity->is_removal_scheduled()) {
        //remove v from vector here
    }
}
*/

#include <time.h>

#include "entities/EntityDefines.h"

class EntityScheduler {

public:
    void schedule_removal() { removal_scheduled = true; }
    void schedule_removal_in(float ms) { removal_start_time = clock(); removal_in_ms = ms; }
    bool is_removal_scheduled() { return removal_scheduled; }

protected:
    bool removal_scheduled = false;
    clock_t removal_start_time;
    float removal_in_ms = 0.0f;

    void update_scheduler() {
        if (removal_in_ms != 0.0f && clock() - removal_start_time >= removal_in_ms) schedule_removal();
    }

private:
    EntityScheduler() { }
};

#endif