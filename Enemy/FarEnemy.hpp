#ifndef FARENEMY_HPP
#define FARENEMY_HPP
#include "Enemy.hpp"
#include <random>

class FarEnemy : public Enemy {
public:
    float reload; // 子彈冷卻計時器
    float rotateRadian = 4 * ALLEGRO_PI; // 最大旋轉弧度/秒
    Turret *TargetTurret = nullptr;
    std::list<FarEnemy *>::iterator lockedEnemyIterator; // FarEnemy iterator in the lockedEnemys list of the target turret.

    FarEnemy(int x, int y);
    void Update(float deltaTime) override;
    void CreateBullet() override;
    void OnExplode() override;
};


#endif   // FARENEMY_HPP
