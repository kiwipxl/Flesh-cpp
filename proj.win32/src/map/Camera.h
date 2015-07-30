#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../SceneManager.h"

namespace map {

    class Camera {

        public:
            Camera();

            cc::Camera* cam_node;

            void update();
    };
};

#endif