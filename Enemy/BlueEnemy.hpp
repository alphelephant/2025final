#ifndef BLUEENEMY_HPP
#define BLUEENEMY_HPP
#include "Enemy.hpp"

class BlueEnemy : public Enemy {
private:
    float reload = 0;
    void Attack();
    void OnExplode() override;
public:
    BlueEnemy(float x, float y);
    void Update(float deltaTime) override;
};
#endif