#include "Camera.h"
#include "../StateManager.h"
#include "../entities/Player.h"

using map::Camera;

Camera::Camera() {
    state::scene->setCameraMask((unsigned short)cc::CameraFlag::USER1, true);

    cam_node = cc::Camera::createPerspective(60, (float)state::scene->screen_size.width / state::scene->screen_size.height, 1.0, 1000);
    cam_node->setCameraFlag(cc::CameraFlag::USER1);
    state::scene->addChild(cam_node);
}

void Camera::update() {
    cc::Vec3 pos;
    pos.x = entity::player->base->getPositionX();
    pos.y = entity::player->base->getPositionY();
    cam_node->setPosition3D(pos + cc::Vec3(0, 0, 600));
    cam_node->lookAt(pos, cc::Vec3(0.0, 1.0, 0.0));
}