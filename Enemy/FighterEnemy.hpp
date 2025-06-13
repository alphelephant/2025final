#ifndef FIGHTERENEMY_HPP
#define FIGHTERENEMY_HPP
#include <allegro5/base.h>
#include <list>
#include <string>
#include "Enemy.hpp"
#include "Engine/Sprite.hpp"

class FighterEnemy : public Enemy {
private:
    float reload = 0.0f; // 攻擊冷卻
public:
    FighterEnemy(int x, int y);
    void Update(float deltaTime) override;
};
#endif   // FIGHTERENEMY_HPP
