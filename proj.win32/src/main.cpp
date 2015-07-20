#include "AppDelegate.h"
#include "cocos2d.h"

USING_NS_CC;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tchar.h>
#include <CCStdC.h>

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

void cocos_android_app_init(JNIEnv* env, jobject thiz) {
	LOGD("cocos_android_app_init");
	AppDelegate *pAppDelegate = new AppDelegate();
}
#endif
