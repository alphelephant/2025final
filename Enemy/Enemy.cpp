#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "Bullet/Bullet.hpp"
#include "Enemy.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "Fighter/Fighter.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/ExplosionEffect.hpp"

PlayScene *Enemy::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Enemy::OnExplode() {
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
void Enemy::CreateBullet() {
    // Default implementation does nothing.
    // Override this method in derived classes to create specific enemy bullets.
}
Enemy::Enemy(std::string img, float x, float y, float radius, float speed, float hp, int money) : Engine::Sprite(img, x, y), speed(speed), hp(hp), money(money) {
    maxHp = hp; // Set the maximum HP.
    CollisionRadius = radius; // Set the collision radius.
    reachEndTime = 0;
}
void Enemy::Hit(float damage) {
    hp -= damage;
    if (hp <= 0) {
        OnExplode();
        // Remove all turret's reference to target.
        // 1) 先解除所有 fighter 的鎖定
        for (auto &it : lockedFighters) {
            it->TargetEnemy = nullptr;
            // 若你想也把迭代器清掉，可以再加：
            it->lockedFighterIterator = std::list<Fighter*>::iterator();
        }
        lockedFighters.clear();
        for (auto &it : lockedTurrets)
            it->Target = nullptr;
        for (auto &it : lockedBullets)
            it->Target = nullptr;
        getPlayScene()->EarnMoney(money);
        getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("explosion.wav");
    }
}
void Enemy::UpdatePath(const std::vector<std::vector<int>> &mapDistance) {
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
    if (x < 0) x = 0;
    if (x >= PlayScene::MapWidth) x = PlayScene::MapWidth - 1;
    if (y < 0) y = 0;
    if (y >= PlayScene::MapHeight) y = PlayScene::MapHeight - 1;
    Engine::Point pos(x, y);
    int num = mapDistance[y][x];
    //Engine::LOG(Engine::INFO) << num << " " << pos.x << " " << pos.y;
    if (num == -1) {
        num = 0;
        Engine::LOG(Engine::ERROR) << "Enemy path finding error";
    }
    path = std::vector<Engine::Point>(num + 1);
    while (num != 0) {
        std::vector<Engine::Point> nextHops;
        for (auto &dir : PlayScene::directions) {
            int x = pos.x + dir.x;
            int y = pos.y + dir.y;
            if (x < 0 || x >= PlayScene::MapWidth || y < 0 || y >= PlayScene::MapHeight || mapDistance[y][x] != num - 1)
                continue;
            nextHops.emplace_back(x, y);
        }
        // Choose arbitrary one.
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, nextHops.size() - 1);
        pos = nextHops[dist(rng)];
        path[num] = pos;
        //Engine::LOG(Engine::INFO) << num << " " << pos.x << " " << pos.y;
        num--;
    }
    path[0] = PlayScene::EndGridPoint;
}
void Enemy::Update(float deltaTime) {
    PlayScene* scene = getPlayScene();
    // Pre-calculate the velocity.
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
    float remainSpeed = speed * deltaTime;
    // 只要還有移動距離就持續移動
    while (remainSpeed != 0) {
        if (path.empty()) {
            // Reach end point.
            Hit(hp);
            getPlayScene()->Hit();
            reachEndTime = 0;
            return;
        }
        // 取得下一個目標點（格子中心）
        Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
        Engine::Point vec = target - Position;
        // Add up the distances:
        // 1. to path.back()
        // 2. path.back() to border
        // 3. All intermediate block size
        // 4. to end point

        // 計算剩餘到終點的時間
        reachEndTime = (vec.Magnitude() + (path.size() - 1) * PlayScene::BlockSize - remainSpeed) / speed;
        Engine::Point normalized = vec.Normalize();
        // 如果這一幀可以走到目標點
        if (remainSpeed - vec.Magnitude() > 0) {
            Position = target;
            path.pop_back();
            remainSpeed -= vec.Magnitude();
        } else {
            Velocity = normalized * remainSpeed / deltaTime;
            remainSpeed = 0;
        }
    }
    // 設定旋轉角度
    Rotation = atan2(Velocity.y, Velocity.x);
    if(isFighterEnemy){
        // 檢查範圍內是否有任何Fighter
        for (auto& obj : scene->FighterGroup->GetObjects()) {
            auto fighter = dynamic_cast<Fighter*>(obj);
            if (!fighter) continue;
            fighter->lockedEnemy.push_back(this);
            Engine::Point diff = fighter->Position - Position;
            if (diff.Magnitude() <= attackRange) {
                // 停止移動
                Velocity = Engine::Point(0,0);
                reload -= deltaTime;
                if(reload<=0){
                    fighter->Hit(damage);
                    AudioHelper::PlayAudio("explosion.wav");
                    reload = coolDown;
                    scene->EffectGroup->AddNewObject(new ShockwaveEffect(Position.x, Position.y, attackRange));
                }
                
            }
        }
    }
    Sprite::Update(deltaTime);
}
void Enemy::Draw() const {
    Sprite::Draw();
    if (PlayScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }
    // 血量條
    float barWidth = 40;
    float barHeight = 6;
    float hpPercent = std::max(hp, 0.0f) / maxHp; // 你需要在 Enemy 裡加一個 maxHp 變數
    float left = Position.x - barWidth / 2;
    float top = Position.y - 25;
    // 背景
    al_draw_filled_rectangle(left, top, left + barWidth, top + barHeight, al_map_rgb(100, 100, 100));
    // 血量
    al_draw_filled_rectangle(left, top, left + barWidth * hpPercent, top + barHeight, al_map_rgb(255, 0, 0));
    // 邊框
    al_draw_rectangle(left, top, left + barWidth, top + barHeight, al_map_rgb(0, 0, 0), 2);
}