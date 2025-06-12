#ifndef BLUEENEMY_HPP
#define BLUEENEMY_HPP
#include "Enemy.hpp"

class BlueEnemy : public Enemy {
private:
    float coolDown=0.3f; // 冷卻時間
    float reload=0;
    float triggerRadius;
    float explosionRadius;
    void Destruct();
    void OnExplode()override;
public:
    BlueEnemy(float x, float y);
    void Update(float deltaTime) override;
};
#endif