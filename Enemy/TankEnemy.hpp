#ifndef TANKENEMY_HPP
#define TANKENEMY_HPP
#include "Enemy.hpp"
#include "Engine/Sprite.hpp"

class TankEnemy : public Enemy {
private:
    Sprite head;
    float targetRotation; // 頭部目標旋轉角度
    float bulletCoolDown = 1.0f; // 子彈冷卻計時器
public:
    TankEnemy(int x, int y);
    void Draw() const override;
    void Update(float deltaTime) override;
    void CreateBullet() override;
};
#endif   // TANKENEMY_HPP
