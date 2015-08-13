#include "map/MapCamera.h"

#include "entities/Unit.h"
#include "StateManager.h"

namespace map {

    using namespace cocos2d;

    MapCamera::MapCamera() {
        root::scene->setCameraMask((u_short)CameraFlag::DEFAULT);
        cam = Camera::createPerspective(60, root::scene->screen_size.width / root::scene->screen_size.height, 1.0f, 1000.0f);
        cam->setCameraFlag(CameraFlag::DEFAULT);
        cam->setCameraMask((u_short)CameraFlag::DEFAULT);
        Camera::getDefaultCamera()->setCameraFlag(CameraFlag::USER1);
        root::scene->addChild(cam);
    }

    void MapCamera::update() {
        Vec3 pos;
        pos.x = entities::test_player->base->getPositionX();
        pos.y = entities::test_player->base->getPositionY();
        pos.z = 600;
        cam->setPosition3D(pos);
    }
};