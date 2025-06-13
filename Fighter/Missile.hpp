#ifndef MISSILE_HPP
#define MISSILE_HPP
#include "Fighter.hpp"

class Missile : public Fighter {
public:
    Missile(float x, float y);
    static const int Price = 100; // The price of the missile.
    void AttackEnemy(Enemy *enemy) override;
    void ApproachTarget(float deltaTime) override;
    void UpdatePath(const std::vector<std::vector<int>> &mapDistance, int endx, int endy) override;


};
#endif   // MISSILE_HPP