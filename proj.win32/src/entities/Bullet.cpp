#include "entities/Bullet.h"

#include "assets/Textures.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_BULLET_NS

//public

//private
extern std::vector<BulletPtr> bullets;
    
BulletPtr create_bullet(BulletType type, int x, int y) {
    BulletPtr b(new Bullet(type, x, y));
}

void update() {
    for (int n = 0; n < bullets.size(); ++n) {
        bullets[n]->update();
    }
}

//-- begin Bullet class --

Bullet::Bullet(BulletType type, int x, int y) {
    base = Sprite::create();
    base->setTexture(assets::textures::test_bullet);
}

Bullet::~Bullet() {

}

void Bullet::update() {

}

//-- end Bullet class --

END_BULLET_NS
END_ENTITIES_NS