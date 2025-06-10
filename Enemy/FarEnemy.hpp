#ifndef FARENEMY_HPP
#define FARENEMY_HPP
#include "Enemy.hpp"

class FarEnemy : public Enemy {
public:
    FarEnemy(int x, int y);
    void CreateBullet() override;
};


#endif   // FARENEMY_HPP
