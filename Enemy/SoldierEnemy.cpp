#include <string>
#include "SoldierEnemy.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
SoldierEnemy::SoldierEnemy(int x, int y) 
  : Enemy("play/enemy-1.png", x, y, 20, 100, 10, 25) {
    isFighterEnemy = true; // 設定為 FighterEnemy
}
FlyEnemy::FlyEnemy(int x, int y) 
  : Enemy("play/enemy-2.png", x, y, 20, 300, 25, 25) {

}

// float radius, float speed, float hp, int money