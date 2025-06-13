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
    float damage;
    float speed;
    float hp, maxHp;
    float attackRange;
    float attackSpeed;
    float damageRange;
    float reload = 0;
    bool isSwordFighter = false; // 是否為劍士
    Engine::Point lastGrid; // 上一個格子
    Engine::Point goalGrid;
    PlayScene *getPlayScene();
    //virtual void OnExplode();
   

public:
    Enemy *TargetEnemy = nullptr;
    float reachEndTime;
    std::list<Fighter *>::iterator lockedFighterIterator;
    std::list<Enemy *> lockedEnemy;
    std::vector<std::vector<int>> FindPath;
    //std::list<Fighter *> lockedFighter;
    Fighter(std::string img, float x, float y, float radius, float speed, float hp, float attackRange, float damageRange,float attackSpeed);
    void Hit(float damage);
    void UpdatePath(const std::vector<std::vector<int>> &mapDistance, int endx, int endy);
    void Update(float deltaTime) override;
    virtual void AttackEnemy(Enemy *enemy);
    void AttackBase();
    void Draw() const override;
    void ApproachTarget(float deltaTime);
};
#endif   // FIGHTER_HPP
