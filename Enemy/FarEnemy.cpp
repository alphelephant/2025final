#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "EnemyBullet/EnemyMissileBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"

#include "FarEnemy.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
FarEnemy::FarEnemy(int x, int y) 
    : Enemy("play/enemy-10.png", x, y, 10, 20, 200, 25) {
    // 設定 bulletCoolDown 為 5~20 秒的隨機值
    static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_real_distribution<float> dist(0.0f, 20.0f);
    bulletCoolDown = dist(rng);
        Anchor.y += 8.0f / GetBitmapHeight();
        maxHp = hp;
} //float radius, float speed, float hp, int money
void FarEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime); // 保留原本移動等邏輯
    PlayScene *scene = getPlayScene();
    
    // 檢查是否有目標
    if (TargetTurret) {
        Engine::Point diff = TargetTurret->Position - Position;
        if (diff.Magnitude() > attackRange) {
            TargetTurret->lockedFarEnemys.erase(lockedEnemyIterator);
            TargetTurret = nullptr;
            lockedEnemyIterator = std::list<FarEnemy *>::iterator();
        }
    }
    // 如果沒有目標，尋找新的目標
    if (!TargetTurret ) {
        // Lock first seen target.
        // Can be improved by Spatial Hash, Quad Tree, ...
        // However simply loop through all enemies is enough for this program.
        for (auto &it : scene->TowerGroup->GetObjects()) {
            Turret* turret = dynamic_cast<Turret*>(it);
            if (!turret || !turret->Visible) continue; 
            Engine::Point diff = turret/*it*/->Position - Position;
            if (diff.Magnitude() <= attackRange) {
                TargetTurret = /*dynamic_cast<Turret *>(it); ... =*/ turret;
                TargetTurret->lockedFarEnemys.push_back(this);
                lockedEnemyIterator = std::prev(TargetTurret->lockedFarEnemys.end());
                break;
            }
        }
    }
    if (TargetTurret) {
        Engine::Point originRotation = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
        Engine::Point targetRotation = (TargetTurret->Position - Position).Normalize();
        float maxRotateRadian = rotateRadian * deltaTime;
        float cosTheta = originRotation.Dot(targetRotation);
        // Might have floating-point precision error.
        if (cosTheta > 1) cosTheta = 1;
        else if (cosTheta < -1) cosTheta = -1;
        float radian = acos(cosTheta);
        //if(!Locked) {
            Engine::Point rotation;
            if (abs(radian) <= maxRotateRadian)
                rotation = targetRotation;
            else
                rotation = ((abs(radian) - maxRotateRadian) * originRotation + maxRotateRadian * targetRotation) / radian;
            // Add 90 degrees (PI/2 radian), since we assume the image is oriented upward.
            Rotation = atan2(rotation.y, rotation.x) + ALLEGRO_PI / 2;
        //}

        // 更新子彈冷卻計時器
        bulletCoolDown -= deltaTime;
        if (bulletCoolDown <= 0) {
            CreateBullet();
            bulletCoolDown = 12.0f; // 12.0秒發射一次，可依需求調整
        }
    }
}
void FarEnemy::CreateBullet() {
    Engine::Point diff;
    if (TargetTurret) {
        diff = TargetTurret->Position - Position;
    } else {
        // If no target turret, just shoot in the current direction.
        diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    }
    Engine::Point normalized = diff.Normalize();
    float rotation = atan2(normalized.y, normalized.x);
    
    // Change bullet position to the front of the gun barrel.
    Engine::Point spawnPos = Position + normalized * 36;
    getPlayScene()->EnemyBulletGroup->AddNewObject(new EnemyMissileBullet(spawnPos, normalized, rotation, this));
    AudioHelper::PlayAudio("missile.wav");
}
void FarEnemy::OnExplode() {
    if (TargetTurret) {
        TargetTurret->lockedFarEnemys.erase(lockedEnemyIterator);
        TargetTurret = nullptr;
        lockedEnemyIterator = std::list<FarEnemy*>::iterator();
    }
    Enemy::OnExplode();
}