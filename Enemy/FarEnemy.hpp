#ifndef FARENEMY_HPP
#define FARENEMY_HPP
#include "Enemy.hpp"

class FarEnemy : public Enemy {
public:
    float bulletCoolDown = 1.5f; // 子彈冷卻計時器
    float rotateRadian = 2 * ALLEGRO_PI; // 最大旋轉弧度
    float attackRange = 400.0f;  // 新增射程
    Turret *TargetTurret = nullptr;
    std::list<FarEnemy *>::iterator lockedEnemyIterator; // FarEnemy iterator in the lockedEnemys list of the target turret.

    FarEnemy(int x, int y);
    void Update(float deltaTime) override;
    void CreateBullet() override;
    void OnExplode() override;
};


#endif   // FARENEMY_HPP
