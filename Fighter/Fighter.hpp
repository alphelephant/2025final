#ifndef FIGHTER_HPP
#define FIGHTER_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

class Bullet;
class PlayScene;
class Turret;
class Enemy;

class Fighter : public Engine::Sprite {
protected:
    std::vector<Engine::Point> path;
    float speed;
    float hp;
    float attackRange;
    float attackSpeed;
    float damageRange;
    float reload = 0;
    PlayScene *getPlayScene();
    //virtual void OnExplode();
   

public:
    Enemy *TargetEnemy = nullptr;
    float reachEndTime;
    std::list<Fighter *>::iterator lockedFighterIterator;
    std::list<Enemy *> lockedEnemy;
    //std::list<Fighter *> lockedFighter;
    Fighter(std::string img, float x, float y, float radius, float speed, float hp, float attackRange, float damageRange,float attackSpeed);
    void Hit(float damage);
    void UpdatePath(const std::vector<std::vector<int>> &mapDistance);
    void Update(float deltaTime) override;
    void AttackEnemy(Enemy *enemy);
    void AttackBase();
    void Draw() const override;
    void ApproachTarget(float deltaTime);
};
#endif   // FIGHTER_HPP
