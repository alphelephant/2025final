#include <string>

#include "SwordFighter.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
SwordFighter::SwordFighter(float x, float y) : Fighter("play/enemy-5.png", x, y,
    /*detactradian*/25, /*speed*/250,
    /*hp*/ 100,
    /*attack range*/ 100 ,/*damageRange*/100, /*attackspeed*/ 0.5f) {
    damage = 50;
}