#ifndef SUICIDEENEMY_HPP
#define SUICIDEENEMY_HPP
#include "Enemy.hpp"

class SuicideEnemy : public Enemy {
private:
    float triggerRadius;
    float explosionRadius;
    void SelfDestruct();
    void OnExplode() override;
    bool isExploded = false;
public:
    SuicideEnemy(float x, float y);
    void Update(float deltaTime) override;
};
#endif