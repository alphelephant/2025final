#include <allegro5/base.h>
#include <cmath>
#include <string>
#include <random>

#include "EnemyBullet/EnemyFireBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"

#include "TankEnemy.hpp"

TankEnemy::TankEnemy(int x, int y)
    : Enemy("play/enemy-3.png", x, y, 20, 60, 500, 60),
    head("play/enemy-3-head.png", x, y), targetRotation(0) {
        static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
        std::uniform_real_distribution<float> dist(0.0f, 12.0f);
        bulletCoolDown = dist(rng);
        maxHp = hp; // 設定最大生命值
} //float radius, float speed, float hp, int money
void TankEnemy::Draw() const {
    Enemy::Draw();
    head.Draw();
}
void TankEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    head.Position = Position;
    // Choose arbitrary one.
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(0.0f, 0.7f);
    float rnd = dist(rng);
    if (rnd < deltaTime) {
        // Head arbitrary rotation.
        std::uniform_real_distribution<> distRadian(-2*ALLEGRO_PI, 2*ALLEGRO_PI);
        targetRotation = distRadian(rng);
    }
    head.Rotation = (head.Rotation + deltaTime * targetRotation) / (1 + deltaTime);
    
    bulletCoolDown -= deltaTime;
    if (bulletCoolDown <= 0) {
        CreateBullet();
        bulletCoolDown = 8.0f; // 8.0秒發射一次，可依需求調整
    }
}
void TankEnemy::CreateBullet() {
    // 以目前頭部方向發射
    float basedir = head.Rotation;
    static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<int> dist(1, 3);
    int x = dist(rng);
    for(int i = -1*x + 1; i < x; i++){
        Engine::Point forward = Engine::Point(cos(basedir + i*ALLEGRO_PI/14), sin(basedir + i*ALLEGRO_PI/14));
        Engine::Point spawnPos = Position + forward * 36; // 子彈生成在坦克頭前方
        float rotation = atan2(forward.y, forward.x);
        getPlayScene()->EnemyBulletGroup->AddNewObject(new EnemyFireBullet(spawnPos, forward, rotation, this));
    }

    AudioHelper::PlayAudio("gun.wav");
}