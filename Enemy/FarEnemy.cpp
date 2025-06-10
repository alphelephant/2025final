#include <string>
#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "EnemyBullet/EnemyFireBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "FarEnemy.hpp"
#include "Scene/PlayScene.hpp"

#include "FarEnemy.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
FarEnemy::FarEnemy(int x, int y) : Enemy("play/enemy-10.png", x, y, 10, 100, 5, 25) {
}
void FarEnemy::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new EnemyFireBullet(Position + normalized * 36, diff, rotation, this));
    AudioHelper::PlayAudio("gun.wav");
}