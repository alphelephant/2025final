#include <string>

#include "TankFighter.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
TankFighter::TankFighter(float x, float y) : Fighter("play/enemy-4.png", x, y, 
    /*detactradian*/25, /*speed*/150,
    /*hp*/ 10,
    /*attack range*/ 100 ,/*damageRange*/100, /*attackspeed*/ 0.5f) {
    damage = 50;
}