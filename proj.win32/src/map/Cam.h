#ifndef _CAM_H_
#define _CAM_H_

#include <2d/CCCamera.h>

#include "entities/EntityDefines.h"
#include "map/MapDefines.h"

#define BEGIN_CAMERA_NS     namespace camera {
#define END_CAMERA_NS       };

BEGIN_MAP_NS
BEGIN_CAMERA_NS

namespace cc = cocos2d;

extern cc::Camera* map_cam;
extern cc::Camera* static_cam;

extern void init();
extern void update_game_cam();
extern void follow_bullet(entities::bullets::BulletGroupPtr& bullet);

END_CAMERA_NS
END_MAP_NS

#endif