#ifndef FIGHTERENEMY_HPP
#define FIGHTERENEMY_HPP
#include "Enemy.hpp"

class fighterEnemy : public Enemy {
private:
    float coolDown=0.3;
    float reload=0;
    float triggerRadius;
    float explosionRadius;
    void SelfDestruct();
    void OnExplode()override;
public:
    fighterEnemy(float x, float y);
    void Update(float deltaTime) override;
};
#endif