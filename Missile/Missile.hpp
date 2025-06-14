#ifndef MISSILE_HPP
#define MISSILE_HPP
#include <string>

#include "Engine/Sprite.hpp"

class Enemy;
class PlayScene;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class Missile : public Engine::Sprite {
protected:
    float speed ;
    float damage;
    float attackRange = 1500; // Attack range of the missile.
    PlayScene *getPlayScene();
    virtual void OnExplode(Enemy *enemy);
    std::list<Missile *>::iterator lockedMissileIterator;

public:
    Enemy *Target = nullptr;
    Missile(std::string img, float speed, float damage, Engine::Point position, float rotation);
    void Update(float deltaTime) override;
};
#endif   // MISSILE_HPP