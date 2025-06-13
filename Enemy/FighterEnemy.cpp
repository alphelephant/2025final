#include <cmath>
#include <string>
#include "FighterEnemy.hpp"
#include "Fighter/Fighter.hpp"
#include "Engine/AudioHelper.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/ExplosionEffect.hpp"

FighterEnemy::FighterEnemy(int x, int y)
  : Enemy("play/enemy-7.png", x, y, 20, 150, 210, 25) {
    // float radius, float speed, float hp, int money
    damage = 20; // 設定傷害
    detectRange = 120; // 偵測範圍
    attackRange = 120; // 攻擊範圍
    coolDown = 0.5f; // 攻擊冷卻時間
    isFighterEnemy = true; // 設定為 FighterEnemy
}

/*void FighterEnemy::Update(float deltaTime) {
    PlayScene* scene = getPlayScene();
    bool foundFighter = false;

    // 檢查範圍內是否有任何Fighter
    for (auto& obj : scene->FighterGroup->GetObjects()) {
        auto fighter = dynamic_cast<Fighter*>(obj);
        if (!fighter) continue;
        Engine::Point diff = fighter->Position - Position;
        if (diff.Magnitude() <= detectRange) {
            foundFighter = true;
            break;
        }
    }

    if (foundFighter) {
        // 停止移動
        Velocity = Engine::Point(0, 0);
        reload -= deltaTime;
        if (reload <= 0) {
            // 範圍攻擊所有Fighter
            for (auto& obj : scene->FighterGroup->GetObjects()) {
                if (!obj) continue;
                auto fighter = dynamic_cast<Fighter*>(obj);
                if (!fighter) continue;
                Engine::Point diff = fighter->Position - Position;
                if (diff.Magnitude() <= detectRange) {
                    fighter->Hit(damage);
                }
            }
            // 播放特效與音效
            scene->EffectGroup->AddNewObject(new ShockwaveEffect(Position.x, Position.y, attackRange));
            AudioHelper::PlayAudio("explosion.wav");
            reload = coolDown; // 重置冷卻時間
        }
    } else {
        // 沒有Fighter則照常移動
        Enemy::Update(deltaTime);
    }
}*/