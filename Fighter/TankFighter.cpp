#include <string>

#include "TankFighter.hpp"
#include "Engine/AudioHelper.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
TankFighter::TankFighter(float x, float y) : Fighter("play/enemy-4.png", x, y, 
    /*detactradian*/25, /*speed*/200,
    /*hp*/ 350,
    /*attack range*/ 100 ,/*damageRange*/110, /*attackspeed*/ 2.0f) {
    damage = 30;
}
void TankFighter::AttackEnemy(Enemy *enemy) {
    Fighter::AttackEnemy(enemy);
    AudioHelper::PlayAudio("NeverGonnaGiveYouUp.wav");
}