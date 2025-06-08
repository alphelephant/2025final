#include <cmath>
#include <random>
#include "SuicideEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "Engine/AudioHelper.hpp"
#include "Bullet/Beam.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/ExplosionEffect.hpp"

SuicideEnemy::SuicideEnemy(float x, float y)
  : Enemy("play/enemy-6.png", x, y,
          /*radius=*/20, /*speed=*/500,
          /*hp=*/20, /*money=*/100),
    triggerRadius(200), explosionRadius(350)
{}
void SuicideEnemy::OnExplode(){
    getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    getPlayScene()->EffectGroup->AddNewObject(new ShockwaveEffect(Position.x, Position.y, explosionRadius));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
    for (int i = 0; i < 10; i++) {
        // Random add 10 dirty effects.
        getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
}
void SuicideEnemy::SelfDestruct() {
    if (isExploded) return;  // 如果已經自爆，則不再執行
    isExploded = true;  // 標記為已自爆
    auto scene = getPlayScene();
    //  效果：爆炸動畫與特效
    OnExplode(); 
    AudioHelper::PlayAudio("shockwave.ogg");

    /*//刪子彈  
    for (auto &obj : scene->BulletGroup->GetObjects()) {
        auto beam = dynamic_cast<Beam*>(obj);
        if (beam ) {
            scene->BulletGroup->RemoveObject(beam->GetObjectIterator());
        }
    }*/
    //刪子彈  
    std::vector<Beam*> beamsToRemove;
    auto bulletObjects = scene->BulletGroup->GetObjects();
    for (auto obj : scene->BulletGroup->GetObjects()) {
        auto beam = dynamic_cast<Beam*>(obj);
        if (beam) {
            beamsToRemove.push_back(beam);
        }
    }
    for (auto beam : beamsToRemove) {
        scene->BulletGroup->RemoveObject(beam->GetObjectIterator());
    }
    
    /*// 摧毀範圍內的塔台
    for (auto &obj : scene->TowerGroup->GetObjects()) {
        auto turret = dynamic_cast<Turret*>(obj);
        if (!turret) continue;
        float dist = (turret->Position - Position).Magnitude();
        if (dist <= explosionRadius) {
            int tx = static_cast<int>(turret->Position.x) / 64;
            int ty = static_cast<int>(turret->Position.y) / 64;
            scene->mapState[ty][tx] = PlayScene::TileType::TILE_FLOOR;  // 恢復地面狀態
            //scene->BulletGroup->RemoveObject(turret->GetObjectIterator());
            scene->TowerGroup->RemoveObject(turret->GetObjectIterator());
        }
    }*/
    // ...existing code...
    // 摧毀範圍內的塔台
    std::vector<Turret*> turretsToRemove;
    auto towerObjects = scene->TowerGroup->GetObjects();
    for (auto obj : scene->TowerGroup->GetObjects()) {
        auto turret = dynamic_cast<Turret*>(obj);
        if (!turret) continue;
        float dist = (turret->Position - Position).Magnitude();
        if (dist <= explosionRadius) {
            int tx = static_cast<int>(turret->Position.x) / 64;
            int ty = static_cast<int>(turret->Position.y) / 64;
            scene->mapState[ty][tx] = PlayScene::TileType::TILE_FLOOR;  // 恢復地面狀態
            turretsToRemove.push_back(turret);
        }
    }
    for (auto turret : turretsToRemove) {
        scene->TowerGroup->RemoveObject(turret->GetObjectIterator());
    }
    // ...existing code...

    // 3) 自己也從場上移除
    scene->EnemyGroup->RemoveObject(objectIterator);
}

void SuicideEnemy::Update(float deltaTime) {
    if (isExploded) return;  // 如果已經自爆，則不再更新
    // 1) 照常移動、追蹤路徑
    Enemy::Update(deltaTime);

    // 2) 檢查是否有任一塔台進入 triggerRadius
    auto scene = getPlayScene();
    for (auto &obj : scene->TowerGroup->GetObjects()) {
        auto turret = dynamic_cast<Turret*>(obj);
        if (!turret) continue;
        float dist = (turret->Position - Position).Magnitude();
        if (dist <= triggerRadius) {
            SelfDestruct();
            return;  // 自爆後不再繼續
        }
    }
}