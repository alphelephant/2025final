#include <string>

#include "TankFighter.hpp"
#include "Engine/AudioHelper.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
TankFighter::TankFighter(float x, float y) : Fighter("play/enemy-4.png", x, y, 
    /*detactradian*/25, /*speed*/150,
    /*hp*/ 100,
    /*attack range*/ 150 ,/*damageRange*/150, /*attackspeed*/ 2.0f) {
    damage = 200;
}
void TankFighter::AttackEnemy(Enemy *enemy) {
    Fighter::AttackEnemy(enemy);
    AudioHelper::PlayAudio("NeverGonnaGiveYouUp.wav");
}