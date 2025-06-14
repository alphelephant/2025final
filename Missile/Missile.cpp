#include "Missile.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Scene/PlayScene.hpp"

PlayScene *Missile::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void Missile::OnExplode(Enemy *enemy) {
}
Missile::Missile(std::string img, float speed, float damage, Engine::Point position, float rotation)
  : Sprite(img, position.x, position.y), speed(speed), damage(damage) {
    Rotation = rotation;
    CollisionRadius = 20;
    Tint = al_map_rgba(255, 255, 255, 255);
}
void Missile::Update(float deltaTime) {
    PlayScene *scene = getPlayScene();
    if (Target) {
        Engine::Point diff = Target->Position - Position;
        if (diff.Magnitude() > attackRange) {
            if (lockedMissileIterator != std::list<Missile*>::iterator())
                Target->lockedMissiles.erase(lockedMissileIterator);
            Target = nullptr;
            lockedMissileIterator = std::list<Missile*>::iterator();
        }
    }
    if (!Target) {
        Enemy* nearest = nullptr;
        float minDist = attackRange + 1;
        for (auto& obj : scene->EnemyGroup->GetObjects()) {
            auto enemy = dynamic_cast<Enemy*>(obj);
            if (!enemy) continue;
            float dist = (enemy->Position - Position).Magnitude();
            if (dist <= attackRange && dist < minDist) {
                minDist = dist;
                nearest = enemy;
            }
        }
        if (nearest) {
            Target = nearest;
            Target->lockedMissiles.push_back(this);
            lockedMissileIterator = std::prev(Target->lockedMissiles.end());
        }
    }
    if (Target) {
        Engine::Point diff = Target->Position - Position;
        if (diff.Magnitude() <= CollisionRadius) {
            OnExplode(Target);
            Target->Hit(damage);
            getPlayScene()->MissileGroup->RemoveObject(objectIterator);
            return;
        }
        Engine::Point normalized = diff.Normalize();
        Velocity = normalized * speed ;
        Rotation = atan2(Velocity.y , Velocity.x)- ALLEGRO_PI/2;
    }
    Sprite::Update(deltaTime);
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize()))
        getPlayScene()->MissileGroup->RemoveObject(objectIterator);
}