#include "AppDelegate.h"
#include "State_Manager.h"

USING_NS_CC;

AppDelegate::AppDelegate() { }
AppDelegate::~AppDelegate() { }

static int register_all_packages() {
    return 0; //flag for packages manager
}

void AppDelegate::initGLContextAttrs() {
    //red, green, blue, alpha, depth, stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
	//init director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLViewImpl::create("Worms Game");
        director->setOpenGLView(glview);
    }

	//turn on debugging stats (fps, gl verts, ect)
	director->setDisplayStats(true);

	//set ms per frame (fps)
    director->setAnimationInterval(1.0f / 60.0f);

	register_all_packages();

	auto scene = State_Manager::createScene();
    director->runWithScene(scene);

    return true;
}

//called when app is inactive
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
}

//called when app is active
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
}
