#include "map/Cam.h"

#include "entities/units/Unit.h"
#include "StateManager.h"

BEGIN_MAP_NS
BEGIN_CAMERA_NS

using namespace cocos2d;

Camera* map_cam;
Camera* static_cam;

void init() {
    Camera::getDefaultCamera()->setVisible(false);
    
    map_cam = Camera::createPerspective(60, root::scene->screen_size.width / root::scene->screen_size.height, 1.0f, 10000.0f);
    map_cam->setCameraFlag(CameraFlag::USER2);
    map_cam->setCameraMask((u_short)CameraFlag::USER2);
    root::scene->addChild(map_cam);
    map_cam->setPositionZ(600);

    static_cam = Camera::createPerspective(60, root::scene->screen_size.width / root::scene->screen_size.height, 1.0f, 10000.0f);
    static_cam->setCameraFlag(CameraFlag::USER1);
    static_cam->setCameraMask((u_short)CameraFlag::USER1);
    root::scene->addChild(static_cam);
    static_cam->setPosition3D(Vec3(root::scene->screen_size.width / 2.0f, root::scene->screen_size.height / 2.0f, 600));
}

void update() {

}

END_CAMERA_NS
END_MAP_NS