#include <string>

#include "TankFighter.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
TankFighter::TankFighter(float x, float y) : Fighter("play/enemy-4.png", x, y, 
    /*detactradian*/10, /*speed*/100,
    /*hp*/ 5,
    /*attack range*/ 25, /*attackspeed*/ 1.0f) {
}