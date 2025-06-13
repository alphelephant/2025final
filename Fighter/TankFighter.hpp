#ifndef TANKFIGHTER_HPP
#define TANKFIGHTER_HPP
#include "Fighter.hpp"

class TankFighter : public Fighter {
public:
    TankFighter(float x, float y);
    static const int Price = 100; // The price of the tank fighter.
    void AttackEnemy(Enemy *enemy) override;

};

#endif   // TANKFIGHTER_HPP