#ifndef BEAM_HPP
#define BEAM_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class Beam : public Bullet {
private:
    int hitCount = 0;
public:
    Turret* GetParent() const { return parent; }
    explicit Beam(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy *enemy) override;
    void Update(float deltaTime) override ;
    
};
#endif   // LASERBULLET_HPP
