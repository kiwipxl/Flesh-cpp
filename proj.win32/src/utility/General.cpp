#include "utility/General.h"

#include <base/CCScheduler.h>

#include "StateManager.h"

BEGIN_UTILITY_NS

//private
std::vector<InvokerFn> invoker_fns;

//public
void invoke_main_thread(InvokerFn fn) {
    invoker_fns.push_back(fn);
}

void update_general() {
    if (invoker_fns.size() == 0) return;

    int size = invoker_fns.size();
    for (int n = 0; n < size; ++n) {
        if (invoker_fns[n]) invoker_fns[n]();
    }
    invoker_fns.erase(invoker_fns.begin(), invoker_fns.begin() + size);
}

END_UTILITY_NS
