#ifndef TURRET_HPP
#define TURRET_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Engine/Sprite.hpp"

class Enemy;
class FarEnemy;
class PlayScene;

class Turret : public Engine::Sprite {
protected:
    int price;
    float coolDown;
    float reload = 0;
    float rotateRadian = 2 * ALLEGRO_PI;
    float hp;
    Sprite imgBase;
    std::list<Turret *>::iterator lockedTurretIterator; // Turret iterator in the lockedTurrets list of the target enemy.
    PlayScene *getPlayScene();
    // Reference: Design Patterns - Factory Method.
    virtual void CreateBullet() = 0;

public:
    bool Enabled = true;
    bool Locked = false;
    bool Preview = false;
    bool BeamCoolDown = true;
    float Bodyrange = 25; // The range of the turret body.
    Enemy *Target = nullptr;
    Turret(std::string imgBase, std::string imgTurret, float x, float y, float radius, int price, float coolDown, float hp);
    void Update(float deltaTime) override;
    void Draw() const override;
    int GetPrice() const;
    void Hit(float damage); 
    std::list<FarEnemy *> lockedFarEnemys; // Enemies that are locked by this turret.
};
#endif   // TURRET_HPP
