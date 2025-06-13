#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>

#include "Bullet/Bullet.hpp"
#include "Fighter.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/PlayScene.hpp"
#include "Turret/Turret.hpp"
#include "Enemy/Enemy.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/ExplosionEffect.hpp"

PlayScene *Fighter::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
/*void Fighter::OnExplode() {
    getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(Position.x, Position.y));
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distId(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 20);
    for (int i = 0; i < 10; i++) {
        // Random add 10 dirty effects.
        getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-" + std::to_string(distId(rng)) + ".png", dist(rng), Position.x, Position.y));
    }
}*/
Fighter::Fighter(std::string img, float x, float y, float radius, float speed, float hp, float attackRange,float damageRange, float attackSpeed) : Engine::Sprite(img, x, y), speed(speed), hp(hp), attackRange(attackRange),damageRange(damageRange) , attackSpeed(attackSpeed) {
    CollisionRadius = radius;
    reload = 0;
    maxHp = hp; // 設定最大生命值
    TargetEnemy = nullptr;
    PlayScene* scene = getPlayScene();
    FindPath = scene->FightDistance;
    lastGrid = Engine::Point(x, y);
}
void Fighter::Hit(float damage) {
    hp -= damage;
    if (hp <= 0) {
        // 解除自己在 Enemy 的鎖定
        if (TargetEnemy && lockedFighterIterator != std::list<Fighter*>::iterator()) {
            TargetEnemy->lockedFighters.erase(lockedFighterIterator);
            lockedFighterIterator = std::list<Fighter*>::iterator();
            TargetEnemy = nullptr;
        }
        // ...原本的解除 lockedEnemy...
        // Remove all turret's reference to target.
        for (auto &it : lockedEnemy)
            it = nullptr;
        /*for (auto &it : lockedBullets)
            it->Target = nullptr;*///這邊有點問題之後再改*/
        getPlayScene()->FighterGroup->RemoveObject(objectIterator);
        //AudioHelper::PlayAudio("explosion.wav");
    }
}
void Fighter::UpdatePath(const std::vector<std::vector<int>> &FighterDistance, int endx, int endy) {
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
    if (x < 0) x = 0;
    if (x >= PlayScene::MapWidth) x = PlayScene::MapWidth - 1;
    if (y < 0) y = 0;
    if (y >= PlayScene::MapHeight) y = PlayScene::MapHeight - 1;
    Engine::Point pos(x, y);
    int num = FighterDistance[y][x];
    if (num == -1) {
        num = 0;
        Engine::LOG(Engine::ERROR) << "Fighter path finding error";
    }
    path = std::vector<Engine::Point>(num + 1);
    while (num != 0) {
        std::vector<Engine::Point> nextHops;
        for (auto &dir : PlayScene::directions) {
            int x = pos.x + dir.x;
            int y = pos.y + dir.y;
            if (x < 0 || x >= PlayScene::MapWidth || y < 0 || y >= PlayScene::MapHeight || FindPath[y][x] != num - 1||  getPlayScene()->mapState[y][x]!=PlayScene::TileType::TILE_DIRT)
                continue;
            nextHops.emplace_back(x, y);
        }
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, nextHops.size() - 1);
        pos = nextHops[dist(rng)]; 
        path[num] = pos;
        num--;
    }
    path[0] = Engine::Point(endx, endy);
}
void Fighter::Update(float deltaTime) {
    PlayScene* scene = getPlayScene();
    float remainSpeed = speed * deltaTime;
    while (remainSpeed != 0 && !path.empty()) {
        int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
        int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
        if (x <= 0 && y <= 1) {
            // Reach end point.
            Hit(hp);
            getPlayScene()->Hitenemy();
            reachEndTime = 0;
            return;
        }
        //Engine::Point nowpos(x*PlayScene::BlockSize + PlayScene::BlockSize / 2, y * PlayScene::BlockSize + PlayScene::BlockSize / 2);
        Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
        Engine::Point vec = target - Position;
        reachEndTime = (vec.Magnitude() + (path.size() - 1) * PlayScene::BlockSize - remainSpeed) / speed;
        Engine::Point normalized = vec.Normalize();
        if (remainSpeed >= vec.Magnitude()) {
            Position = target;
            path.pop_back();
            remainSpeed -= vec.Magnitude();
        } else {
            Velocity = normalized * remainSpeed / deltaTime;
            remainSpeed = 0;
        }
    }
    Rotation = atan2(Velocity.y, Velocity.x);

    if (TargetEnemy) {
        Engine::Point diff = TargetEnemy->Position - Position;
        if (diff.Magnitude() > attackRange) {
            if (lockedFighterIterator != std::list<Fighter*>::iterator())
                TargetEnemy->lockedFighters.erase(lockedFighterIterator);
            TargetEnemy = nullptr;
            lockedFighterIterator = std::list<Fighter*>::iterator();
        }
    }
    
    // 2. 如果沒有目標，尋找最近的敵人
    if (!TargetEnemy) {
        for (auto& obj : scene->EnemyGroup->GetObjects()) {
            auto enemy = dynamic_cast<Enemy*>(obj);
            if (!enemy) continue;
            Engine::Point diff = enemy->Position - Position;
            if (diff.Magnitude() <= attackRange) {
                TargetEnemy = enemy;
                TargetEnemy->lockedFighters.push_back(this);
                lockedFighterIterator = std::prev(TargetEnemy->lockedFighters.end());
                break;
            }
        }
    }
    if (TargetEnemy) {
        reload -= deltaTime;
        Velocity = Engine::Point(0, 0); // 停止移動
        if (reload <= 0) {
            Engine::Point diff = TargetEnemy->Position - Position;
            if (diff.Magnitude() <= damageRange){
                reload = attackSpeed;
                AttackEnemy(TargetEnemy);
            }
        }
    }
    Sprite::Update(deltaTime);
}

void Fighter::AttackEnemy(Enemy *enemy) {
    if (enemy) {
        if(!isSwordFighter)
            getPlayScene()->EffectGroup->AddNewObject(new ShockwaveEffect(Position.x, Position.y, damageRange));
        enemy->Hit(damage); // Assuming each fighter does 10 damage.
    }
}
void Fighter::Draw() const {
    Sprite::Draw();
    if (PlayScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, attackRange, al_map_rgb(0, 0, 255), 2);
    }
    // 血量條
    float barWidth = 40;
    float barHeight = 6;
    float hpPercent = std::max(hp, 0.0f) / maxHp;
    float left = Position.x - barWidth / 2;
    float top = Position.y+25;
    // 背景
    al_draw_filled_rectangle(left, top, left + barWidth, top + barHeight, al_map_rgb(100, 100, 100));
    // 血量
    al_draw_filled_rectangle(left, top, left + barWidth * hpPercent, top + barHeight, al_map_rgb(0, 128, 255));
    // 邊框
    al_draw_rectangle(left, top, left + barWidth, top + barHeight, al_map_rgb(0, 0, 0), 2);
}
void Fighter::ApproachTarget(float deltaTime) {
    // 預設什麼都不做
}