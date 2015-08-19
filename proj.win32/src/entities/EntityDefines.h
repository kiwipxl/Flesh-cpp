#ifndef _ENTITY_DEFINES_H_
#define _ENTITY_DEFINES_H_

#define BEGIN_ENTITIES_NS           namespace entities {
#define END_ENTITIES_NS             };

#define BEGIN_UNITS_NS              namespace units {
#define END_UNITS_NS                };

#define BEGIN_COMPONENTS_NS         namespace components {
#define END_COMPONENTS_NS           };

#define BEGIN_BULLETS_NS            namespace bullets {
#define END_BULLETS_NS              };

//forward declarations begin
BEGIN_ENTITIES_NS
BEGIN_BULLETS_NS
class Bullet;
typedef std::shared_ptr<Bullet> BulletPtr;

class BulletGroup;
typedef std::shared_ptr<BulletGroup> BulletGroupPtr;
END_BULLETS_NS

BEGIN_UNITS_NS
class Unit;
END_UNITS_NS
END_ENTITIES_NS
//forward declarations end

#endif