#include <string>

#include "Missile.hpp"
#include "Engine/AudioHelper.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
Missile::Missile(float x, float y) : Fighter("play/enemy-4.png", x, y,
    /*detactradian*/25, /*speed*/150,
    /*hp*/ 200,
    /*attack range*/ 150 ,/*damageRange*/150, /*attackspeed*/ 2.0f) {
    damage = 200;
}
void Missile::AttackEnemy(Enemy *enemy) {
    Fighter::AttackEnemy(enemy);
    AudioHelper::PlayAudio("NeverGonnaGiveYouUp.wav");
}