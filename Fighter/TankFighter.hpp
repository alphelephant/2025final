#ifndef TANKFIGHTER_HPP
#define TANKFIGHTER_HPP
#include "Fighter.hpp"

class TankFighter : public Fighter {
public:
    TankFighter(float x, float y);
    static const int Price = 500; // The price of the tank fighter.
};

#endif   // TANKFIGHTER_HPP