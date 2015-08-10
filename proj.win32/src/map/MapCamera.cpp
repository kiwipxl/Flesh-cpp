#include "map/MapCamera.h"

#include <2d/CCCamera.h>
#include "cocos2d.h"

#include "entities/Unit.h"
#include "StateManager.h"

namespace map {

    using namespace cocos2d;

    MapCamera::MapCamera() {
        root::scene->setCameraMask((u_short)CameraFlag::DEFAULT);
        cam_node = Camera::createPerspective(60, root::scene->screen_size.width / root::scene->screen_size.height, 1.0f, 1000.0f);
        cam_node->setCameraFlag(CameraFlag::DEFAULT);
        cam_node->setCameraMask((u_short)CameraFlag::DEFAULT);
        Camera::getDefaultCamera()->setCameraFlag(CameraFlag::USER1);
        root::scene->addChild(cam_node);
    }

    void MapCamera::update() {
        Vec3 pos;
        pos.x = entities::test_player->base->getPositionX();
        pos.y = entities::test_player->base->getPositionY();
        pos.z = 600;
        cam_node->setPosition3D(pos);
    }
};