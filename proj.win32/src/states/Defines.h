#ifndef _STATES_DEFINES_H_
#define _STATES_DEFINES_H_

#define BEGIN_STATES_NS     namespace states {
#define END_STATES_NS       }

#define SEP ,
#define INVOKE_MAIN_THREAD(capture)     root::scene->getScheduler()->performFunctionInCocosThread([capture]()

#endif