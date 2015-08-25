#include "utility/General.h"

#include <base/CCScheduler.h>

#include "StateManager.h"

BEGIN_UTILITY_NS

//private

//public
void invoke_main_thread(std::function<void()> fn) {
    root::scene->getScheduler()->performFunctionInCocosThread(fn);
}

END_UTILITY_NS
