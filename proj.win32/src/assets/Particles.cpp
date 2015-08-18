#include "Particles.h"

#include "debug/Logger.h"

BEGIN_ASSETS_NS

namespace particles {

	using namespace cocos2d;

	//private
	ParticleSystem* bullet_explosion;
    std::string bullet_explosion_name = "fireballpls.plist";
    std::string bullet_fire_trail = "bullet_fire_trail.plist";

	void load_particle(ParticleSystem*& particle, std::string file_name) {
		particle = ParticleSystem::create(file_name);
		particle->retain();
	}

	void init() {
		load_particle(bullet_explosion, bullet_explosion_name);

		log_print_file << "particle assets loaded";
	}
};

END_ASSETS_NS
