#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include <2d/CCParticleSystem.h>
#include <2d/CCParticleSystemQuad.h>

#include "assets/AssetsDefines.h"

BEGIN_ASSETS_NS

namespace particles {

	namespace cc = cocos2d;

	extern cc::ParticleSystem* bullet_explosion;
    extern std::string bullet_explosion_name;
    extern std::string bullet_fire_trail;
    extern std::string c4_explosion;

	extern void init();
};

END_ASSETS_NS

#endif