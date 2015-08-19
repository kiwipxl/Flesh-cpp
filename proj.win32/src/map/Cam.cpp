#include "map/Cam.h"

#include "entities/bullets/Bullet.h"
#include "entities/bullets/BulletGroup.h"
#include "entities/units/Unit.h"
#include "entities/units/UnitSpawner.h"
#include "input/KeyboardInput.h"
#include "input/MouseInput.h"
#include "StateManager.h"

BEGIN_MAP_NS
BEGIN_CAMERA_NS

using namespace cocos2d;

//private
float dest_zoom = 0;
entities::bullets::BulletGroupPtr target_follow_bullet;

//public
Camera* map_cam;
Camera* static_cam;

void init() {
    Camera::getDefaultCamera()->setVisible(false);
    
    map_cam = Camera::createPerspective(60, root::scene->screen_size.width / root::scene->screen_size.height, 1.0f, 10000.0f);
    map_cam->setCameraFlag(CameraFlag::USER2);
    map_cam->setCameraMask((u_short)CameraFlag::USER2);
    root::scene->addChild(map_cam);
    map_cam->setPositionZ(600);

    static_cam = Camera::createOrthographic(root::scene->screen_size.width, root::scene->screen_size.height, 1.0f, 10000.0f);
    static_cam->setCameraFlag(CameraFlag::USER1);
    static_cam->setCameraMask((u_short)CameraFlag::USER1);
    root::scene->addChild(static_cam);
    static_cam->setPosition3D(Vec3(0, 0, 1));
}

void update_game_cam() {
    if (target_follow_bullet && target_follow_bullet->is_removal_scheduled()) {
        entities::units::next_unit();
        target_follow_bullet = NULL;
    }

    auto& v = map::camera::map_cam->getPosition();
    Vec2 pos;
    if (target_follow_bullet && target_follow_bullet->bullets.size() >= 1) {
        pos.x = target_follow_bullet->min_pos.x + (target_follow_bullet->max_size.width / 2.0f);
        pos.y = target_follow_bullet->min_pos.y + (target_follow_bullet->max_size.height / 2.0f);
    }else {
        pos = entities::units::current_unit->base->getPosition();
    }
    map::camera::map_cam->setPosition(v.x - (v.x - pos.x) / 10.0f, v.y - (v.y - pos.y) / 10.0f);

    if (input::get_mouse_scroll().y <= -1) {
        dest_zoom -= 20.0f;
    }else if (input::get_mouse_scroll().y >= 1) {
        dest_zoom += 20.0f;
    }else if (input::key_down(EventKeyboard::KeyCode::KEY_MINUS)) {
        dest_zoom += 10.0f;
    }else if (input::key_down(EventKeyboard::KeyCode::KEY_EQUAL) || input::get_mouse_scroll().y >= 1) {
        dest_zoom -= 10.0f;
    }
    dest_zoom -= dest_zoom / 8.0f;
    map::camera::map_cam->setPositionZ(map::camera::map_cam->getPositionZ() + dest_zoom);
}

void follow_bullet(entities::bullets::BulletGroupPtr& bullet) {
    target_follow_bullet = bullet;
}

END_CAMERA_NS
END_MAP_NS