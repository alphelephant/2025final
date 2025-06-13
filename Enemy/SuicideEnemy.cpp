#include <cmath>
#include <random>
#include "SuicideEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "Engine/AudioHelper.hpp"
#include "Bullet/Beam.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/ExplosionEffect.hpp"
#include "Fighter/Fighter.hpp"

SuicideEnemy::SuicideEnemy(float x, float y)
  : Enemy("play/enemy-6.png", x, y, 20, 500, 20, 100) {
    // float radius, float speed, float hp, int money
    isFighterEnemy = true; // 設定為 FighterEnemy
    damage = 9999; // 自爆傷害
    detectRange = 200; // 偵測範圍
    attackRange = 350; // 攻擊範圍
    coolDown = 0.5f; // 攻擊冷卻時間
}
void SuicideEnemy::OnExplode(){
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
void SuicideEnemy::SelfDestruct() {
    if (isExploded) return;  // 如果已經自爆，則不再執行
    isExploded = true;  // 標記為已自爆
    auto scene = getPlayScene();
    //  效果：爆炸動畫與特效
    getPlayScene()->EffectGroup->AddNewObject(new ShockwaveEffect(Position.x, Position.y, attackRange));
    OnExplode(); 
    AudioHelper::PlayAudio("shockwave.ogg");

    //對範圍內的塔台造成傷害
    auto towerObjects = scene->TowerGroup->GetObjects();
    for (auto obj : towerObjects) {
        auto turret = dynamic_cast<Turret*>(obj);
        if (!turret) continue;
        float dist = (turret->Position - Position).Magnitude();
        if (dist <= attackRange) {
            turret->Hit(damage);
        }
    }
    // 造成傷害給自己，觸發死亡事件
    Hit(hp);
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
        if (dist <= detectRange) {
            SelfDestruct();
            return;  // 自爆後不再繼續
        }
    }
}