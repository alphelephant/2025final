#ifndef FIGHTERENEMY_HPP
#define FIGHTERENEMY_HPP
#include <allegro5/base.h>
#include <list>
#include <string>
#include "Enemy.hpp"
#include "Engine/Sprite.hpp"

class FighterEnemy : public Enemy {
private:
    float attackRange = 120; // 攻擊範圍
    float attackSpeed = 0.5f; // 攻擊間隔(秒)
    float reload = 1.0f; // 攻擊冷卻
    float damage = 40; // 傷害
public:
    FighterEnemy(int x, int y);
    void Update(float deltaTime) override;
};
#endif   // FIGHTERENEMY_HPP
