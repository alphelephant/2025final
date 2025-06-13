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
    //float CollisionRadius; // 碰撞半徑
    float speed;
    float hp, maxHp;
    int money;
    bool isFighterEnemy = false; // 是否為 FighterEnemy

    float damage = 0;
    float detectRange = 0; // 偵測範圍
    float attackRange = 0; // 攻擊範圍
    float coolDown = 0.0f; // 攻擊冷卻時間
    float reload = 0.0f; // 子彈冷卻計時器
    
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
    float GetInfo(std::string str){
        if (str == "hp") return hp;
        if (str == "maxHp") return maxHp;
        if (str == "speed") return speed;
        if (str == "money") return money;
        if (str == "damage") return damage;
        if (str == "detectRange") return detectRange;
        if (str == "attackRange") return attackRange;
        if (str == "coolDown") return coolDown;
        if (str == "reload") return reload;
        return 0.0f;
    }
};
#endif   // ENEMY_HPP
