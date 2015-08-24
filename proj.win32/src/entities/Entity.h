#ifndef _ENTITY_H_
#define _ENTITY_H_

/*
entities are the base interactable objects
todo: this
*/

#include <vector>

#include "entities/EntityDefines.h"
#include "entities/EntityScheduler.h"

class Entity : public EntityScheduler {

public:
    template <typename T> std::shared_ptr<T>    add_component();
    template <typename T> void  remove_component();
    template <typename T> std::shared_ptr<T>    get_component();

private:
    //std::vector<std::shared_ptr<components::ComponentBase>> components;
};

#endif