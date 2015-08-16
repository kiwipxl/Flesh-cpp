#include "map/MapCamera.h"

#include "entities/Unit.h"
#include "StateManager.h"

namespace map {

    using namespace cocos2d;

    MapCamera::MapCamera() {
        root::scene->setCameraMask((u_short)CameraFlag::DEFAULT);
        cam = Camera::createPerspective(60, root::scene->screen_size.width / root::scene->screen_size.height, 1.0f, 10000.0f);
        cam->setCameraFlag(CameraFlag::DEFAULT);
        cam->setCameraMask((u_short)CameraFlag::DEFAULT);
        Camera::getDefaultCamera()->setCameraFlag(CameraFlag::USER1);
        root::scene->addChild(cam);

        cam->setPositionZ(600);
    }

    void MapCamera::update() {
        cam->setPosition(entities::test_player->base->getPosition());
    }
};