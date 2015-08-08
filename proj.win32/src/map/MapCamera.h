#ifndef _MAP_CAMERA_H_
#define _MAP_CAMERA_H_

#include <2d/CCCamera.h>

namespace map {

    using namespace cocos2d;

    class MapCamera {

        public:
            MapCamera();

            Camera* cam_node;

            void update();
    };
};

#endif