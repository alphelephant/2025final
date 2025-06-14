#include "EnemyBullet.hpp"
#include "Turret/Turret.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Scene/PlayScene.hpp"
#include "Fighter/Fighter.hpp"

PlayScene *EnemyBullet::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
void EnemyBullet::OnExplode(Turret *turret) {
}
EnemyBullet::EnemyBullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Enemy *parent) 
  : Sprite(img, position.x, position.y), speed(speed), damage(damage), parent(parent) {
    Velocity = forwardDirection.Normalize() * speed;
    Rotation = rotation;
    CollisionRadius = 4;
}
void EnemyBullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    PlayScene *scene = getPlayScene();
    // Can be improved by Spatial Hash, Quad Tree, ...
    // However simply loop through all enemies is enough for this program.
    for (auto &it : scene->TowerGroup->GetObjects()) {
        Turret *turret = dynamic_cast<Turret *>(it);
        if (!turret || !turret->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, turret->Position, turret->Bodyrange)) {
            OnExplode(turret);
            turret->Hit(damage);
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }
    for (auto &it : scene->FighterGroup->GetObjects()) {
        Fighter *fighter = dynamic_cast<Fighter *>(it);
        if (!fighter || !fighter->Visible)
            continue;
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, fighter->Position, fighter->CollisionRadius)) {
            //OnExplode(fighter);
            fighter->Hit(damage);
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }
    // Check if out of boundary.
    if (!Engine::Collider::IsRectOverlap(Position - Size / 2, Position + Size / 2, Engine::Point(0, 0), PlayScene::GetClientSize()))
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
}
