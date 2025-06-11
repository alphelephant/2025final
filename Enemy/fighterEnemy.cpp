#include <cmath>
#include <random>
#include "fighterEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "Engine/AudioHelper.hpp"
#include "Bullet/Beam.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/ExplosionEffect.hpp"

fighterEnemy::fighterEnemy(float x, float y)
  : Enemy("play/enemy-9.png", x, y,
          /*radius=*/20, /*speed=*/200,
          /*hp=*/100, /*money=*/10),
    triggerRadius(180), explosionRadius(220)
{}
void fighterEnemy::OnExplode(){
    getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
    for (int i = 0; i < 10; i++) {
        // Random add 10 dirty effects.
        getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
}
void fighterEnemy::Destruct() {
    auto scene = getPlayScene();
    //  效果：爆炸動畫與特效
    getPlayScene()->EffectGroup->AddNewObject(new ShockwaveEffect(Position.x, Position.y, explosionRadius));
    AudioHelper::PlayAudio("shockwave.ogg");

    // 對範圍內的塔台造成傷害
    auto towerObjects = scene->TowerGroup->GetObjects();
    for (auto obj : towerObjects) {
        auto turret = dynamic_cast<Turret*>(obj);
        if (!turret) continue;
        float dist = (turret->Position - Position).Magnitude();
        if (dist <= explosionRadius) {
            
            turret->Hit(5);
        }
    }
    
}

void fighterEnemy::Update(float deltaTime) {
    
    // 1) 
    reload -= deltaTime;
    Enemy::Update(deltaTime);

    // 2) 檢查是否有任一塔台進入 triggerRadius
    auto scene = getPlayScene();
    for (auto &obj : scene->TowerGroup->GetObjects()) {
        auto turret = dynamic_cast<Turret*>(obj);
        if (!turret) continue;
        float dist = (turret->Position - Position).Magnitude();
        if (dist <= triggerRadius && reload <= 0) {
            reload = coolDown; // 重置冷卻時間
            Destruct();
            return;  // 自爆後不再繼續
        }
    }
}