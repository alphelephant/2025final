#include <allegro5/base.h>
#include <random>
#include <string>

#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "Beam.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

class Turret;
static float getdistance(const Engine::Point &p,const Engine::Point &v,const Engine::Point &w) {
    Engine::Point vw = w - v;
    float l2 = vw.Dot(vw);
    if (l2 == 0.0f)          
        return (p - v).Magnitude();
    float t = std::clamp((p - v).Dot(vw) / l2, 0.0f, 1.0f);
    Engine::Point proj = v + vw * t;
    return (p - proj).Magnitude();
}
Beam::Beam(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent) : Bullet("play/laserbeam.png", 0, 200, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
    hitCount = 0;
}
void Beam::OnExplode(Enemy *enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 10);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), enemy->Position.x, enemy->Position.y));
}
void Beam::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene *scene = getPlayScene();
    
    static float lifeTime = 0;
    lifeTime += deltaTime;
    const float maxLifeTime = 1.0f;
    if (lifeTime >= maxLifeTime) {
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        lifeTime = 0;  
        return;
    }
    if (!parent || !parent->Visible) {
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        return;
    }
    Engine::Point Beamstart = parent->Position;
    Engine::Point Beamend   = Position + Position - parent->Position;
    for (auto &it : scene->EnemyGroup->GetObjects()) {
        Enemy *enemy = dynamic_cast<Enemy *>(it);
        if (!enemy->Visible)
            continue;
         float dist = getdistance(enemy->Position, Beamstart, Beamend);
        if (dist < enemy->CollisionRadius) {
            OnExplode(enemy);
            enemy->Hit(damage);
            /*hitCount++;
            if (hitCount >= 5) {
                hitCount = 0;
                scene->BulletGroup->RemoveObject(objectIterator);
                return;
            }*/
            return;
        }
    }

}