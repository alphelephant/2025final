#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <list>
#include <string>
#include <vector>

#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

class Bullet;
class PlayScene;
class Turret;
class Fighter;

class Enemy : public Engine::Sprite {
protected:
    std::vector<Engine::Point> path;
    float speed;
    float hp,maxHp;
    int money;
    float damage;
    float attackRange;
    float attackSpeed;
    PlayScene *getPlayScene();
    virtual void OnExplode();
    virtual void CreateBullet();
    //std::list<Enemy *>::iterator lockedEnemyIterator;

public:
    float reachEndTime;
    std::list<Turret *> lockedTurrets;
    std::list<Bullet *> lockedBullets;
    std::list<Fighter *> lockedFighters;
    Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money);
    void Hit(float damage);
    void UpdatePath(const std::vector<std::vector<int>> &mapDistance);
    virtual void Update(float deltaTime) override;
    void Draw() const override;
};
#endif   // ENEMY_HPP
