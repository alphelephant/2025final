#include <allegro5/base.h>
#include <random>
#include <string>

#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "EnemyFireBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

class Turret;

EnemyFireBullet::EnemyFireBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Enemy *parent) 
    : EnemyBullet("play/bullet-1.png", 500, 2.5, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
}
void EnemyFireBullet::OnExplode(Turret *turret) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-1.png", dist(rng), turret->Position.x, turret->Position.y));
}
