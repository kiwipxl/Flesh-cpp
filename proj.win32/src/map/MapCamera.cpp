#include "map/MapCamera.h"

#include <2d/CCCamera.h>

#include "entities/Unit.h"
#include "StateManager.h"

namespace map {

    using namespace cocos2d;

    MapCamera::MapCamera() {
        root::scene->setCameraMask((u_short)CameraFlag::USER1, true);

        cam_node = Camera::createPerspective(60, (float)root::scene->screen_size.width / root::scene->screen_size.height, 1.0, 1000);
        cam_node->setCameraFlag(CameraFlag::USER1);
        root::scene->addChild(cam_node);
    }

    void MapCamera::update() {
        Vec3 pos;
        pos.x = entities::test_player->base->getPositionX();
        pos.y = entities::test_player->base->getPositionY();
        cam_node->setPosition3D(pos + Vec3(0, 0, 600));
        cam_node->lookAt(pos, Vec3(0.0, 1.0, 0.0));
    }
};