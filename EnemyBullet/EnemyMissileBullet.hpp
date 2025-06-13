#ifndef ENEMYMISSILEBULLET_HPP
#define ENEMYMISSILEBULLET_HPP
#include "EnemyBullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class EnemyMissileBullet : public EnemyBullet {
public:
    explicit EnemyMissileBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Enemy *parent);
    void OnExplode(Turret *turret) override;
};
#endif   // ENEMYMISSILEBULLET_HPP
