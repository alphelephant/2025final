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
Fighter::Fighter(std::string img, float x, float y, float radius, float speed, float hp, float attackRange,float damageRange, float attackSpeed) 
    : Engine::Sprite(img, x, y), speed(speed), hp(hp), attackRange(attackRange),damageRange(damageRange) , attackSpeed(attackSpeed) {
    CollisionRadius = radius;
    reload = 0;
    TargetEnemy = nullptr;
    PlayScene* scene = getPlayScene();
    FindPath = scene->FightDistance;
    lastGrid = Engine::Point(x, y);
}
void Fighter::Hit(float damage) {
    hp -= damage;
    if (hp <= 0) {
        //OnExplode();
        // Remove all turret's reference to target.
        /*for (auto &it : lockedEnemy)
            it->Target = nullptr;
        for (auto &it : lockedBullets)
            it->Target = nullptr;*///這邊有點問題之後再改
        getPlayScene()->FighterGroup->RemoveObject(objectIterator);
        //AudioHelper::PlayAudio("explosion.wav");
    }
}
void Fighter::UpdatePath(const std::vector<std::vector<int>> &FindPath) {
    int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
    int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
    if (x < 0) x = 0;
    if (x >= PlayScene::MapWidth) x = PlayScene::MapWidth - 1;
    if (y < 0) y = 0;
    if (y >= PlayScene::MapHeight) y = PlayScene::MapHeight - 1;
    Engine::Point pos(x, y);
    int num = FindPath[y][x];
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
        // Choose arbitrary one.
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, nextHops.size() - 1);
        pos = nextHops[dist(rng)];
        path[num] = pos;
        num--;
    }
    if(TargetEnemy){
                int tx = std::clamp(
        int(TargetEnemy->Position.x / PlayScene::BlockSize),
        0, PlayScene::MapWidth - 1
        );
        int ty = std::clamp(
        int(TargetEnemy->Position.y / PlayScene::BlockSize),0, PlayScene::MapHeight - 1);
        path[0] = Engine::Point(tx, ty);
    }
    else
        path[0] = Engine::Point((PlayScene::SpawnGridPoint.x +1) , PlayScene::SpawnGridPoint.y  );
}
void Fighter::Update(float deltaTime) {
    float remainSpeed = speed * deltaTime;
    PlayScene* scene = getPlayScene();
    if (TargetEnemy) {
        FindPath = scene->CalculateDistance(TargetEnemy->Position.x / PlayScene::BlockSize, TargetEnemy->Position.y / PlayScene::BlockSize,
            Position.x / PlayScene::BlockSize, Position.y / PlayScene::BlockSize);
        UpdatePath(FindPath);
        Engine::Point diff = TargetEnemy->Position - Position;
        if (diff.Magnitude() > attackRange) {
            if (TargetEnemy && lockedFighterIterator != std::list<Fighter*>::iterator())
                TargetEnemy->lockedFighters.erase(lockedFighterIterator);
            TargetEnemy = nullptr;
            lockedFighterIterator = std::list<Fighter*>::iterator();
        }
    }
    else {
    // 預設向基地前進的路徑
        FindPath = getPlayScene()->FightDistance;
    }
    UpdatePath(FindPath);
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
        if (reload <= 0) {
            Engine::Point diff = TargetEnemy->Position - Position;
            if (diff.Magnitude() <= damageRange){
                reload = attackSpeed;
                AttackEnemy(TargetEnemy);
            }
        }
    }
    while (remainSpeed != 0) {
        int x = static_cast<int>(floor(Position.x / PlayScene::BlockSize));
        int y = static_cast<int>(floor(Position.y / PlayScene::BlockSize));
        if (x <= 0 && y <= 0) { 
            // Reach end point.
            Hit(hp);
            //getPlayScene()->Hit();
            reachEndTime = 0;
            return;
        }
        Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
        Engine::Point vec = target - Position;
        // Add up the distances:
        // 1. to path.back()
        // 2. path.back() to border
        // 3. All intermediate block size
        // 4. to end point
        reachEndTime = (vec.Magnitude() + (path.size() - 1) * PlayScene::BlockSize - remainSpeed) / speed;
        Engine::Point normalized = vec.Normalize();
        if (remainSpeed - vec.Magnitude() > 0) {
            Position = target;
            path.pop_back();
            remainSpeed -= vec.Magnitude();
        } else {
            Velocity = normalized * remainSpeed / deltaTime;
            remainSpeed = 0;
        }
    }
    Rotation = atan2(Velocity.y, Velocity.x);
    Sprite::Update(deltaTime);
}
void Fighter::ApproachTarget(float deltaTime) {
    /*auto e = TargetEnemy;
    Engine::Point diff = e->Position - Position;
    float dist = diff.Magnitude();
    Engine::Point dir = diff / dist;  // Normalize

    // 如果還沒到攻擊距離，就移動
    if (dist > damageRange) {
        Position = Position + dir * speed * deltaTime;
        Rotation = atan2(dir.y, dir.x);
    }*/
    //很抽象的東西
}
void Fighter::AttackEnemy(Enemy *enemy) {
    if (enemy) {
        enemy->Hit(50); // Assuming each fighter does 10 damage.
        AudioHelper::PlayAudio("NeverGonnaGiveYouUp.wav");
    }
}
void Fighter::Draw() const {
    Sprite::Draw();
    if (PlayScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, attackRange, al_map_rgb(255, 0, 0), 2);
    }
}
