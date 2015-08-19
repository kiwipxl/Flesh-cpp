#include "Particles.h"

#include "debug/Logger.h"

BEGIN_ASSETS_NS

namespace particles {

	using namespace cocos2d;

	//private
    std::string bullet_explosion = "particles/fireballpls.plist";
    std::string bullet_fire_trail = "particles/bullet_fire_trail.plist";
    std::string c4_explosion = "particles/c4_explosion.plist";
    std::string ring = "particles/ring.plist";
    std::string health_ring = "particles/health_ring.plist";

	void load_particle(ParticleSystem*& particle, std::string file_name) {
		particle = ParticleSystem::create(file_name);
		particle->retain();
	}

	void init() {
		log_print_file << "particle assets loaded";
	}
};

END_ASSETS_NS
