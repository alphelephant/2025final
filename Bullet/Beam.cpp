#include <allegro5/base.h>
#include <random>
#include <string>

#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "Beam.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Animation/DirtyEffect.hpp"

class Turret;
static float getdistance(const Engine::Point &p,const Engine::Point &v,const Engine::Point &w) {
    Engine::Point vw = w - v;
    float l2 = vw.Dot(vw);
    if (l2 == 0.0f)          
        return (p - v).Magnitude();
    float t = std::clamp((p - v).Dot(vw) / l2, 0.0f, 1.0f);
    Engine::Point proj = v + vw * t;
    return (p - proj).Magnitude();
}
Beam::Beam(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent) 
  : Bullet("play/laserbeam-1.png", 0.1, 200, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
        //speed, damage
    hitCount = 0;
}
void Beam::OnExplode(Enemy *enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 10);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), enemy->Position.x, enemy->Position.y));
}
void Beam::Update(float deltaTime) {
    Sprite::Update(deltaTime);

    animTimer += deltaTime;
    if (animTimer >= FRAME_DURATION) {
        animTimer -= FRAME_DURATION;
        animFrame = (animFrame + 1) % FRAME_COUNT;
    }
    PlayScene *scene = getPlayScene();
    
    static float lifeTime = 0;
    lifeTime += deltaTime;
    const float maxLifeTime = 1.0f;
    if (lifeTime >= maxLifeTime) {
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        lifeTime = 0;  
        return;
    }
    if (!parent || !parent->Visible) {
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        return;
    }
    Engine::Point Beamstart = parent->Position;
    Engine::Point Beamend   = Position + Position - parent->Position;
    for (auto &it : scene->EnemyGroup->GetObjects()) {
        Enemy *enemy = dynamic_cast<Enemy *>(it);
        if (!enemy->Visible)
            continue;
         float dist = getdistance(enemy->Position, Beamstart, Beamend);
        if (dist < enemy->CollisionRadius) {
            OnExplode(enemy);
            enemy->Hit(damage);
            /*hitCount++;
            if (hitCount >= 5) {
                hitCount = 0;
                scene->BulletGroup->RemoveObject(objectIterator);
                return;
            }*/
            return;
        }
    }

}
void Beam::Draw() const {
    int bmpW      = GetBitmapWidth();
    int bmpH      = GetBitmapHeight();
    int frameW    = bmpW / FRAME_COUNT;
    int frameH    = bmpH;

    // 計算 region 左上角 x 座標
    int srcX = animFrame * frameW;

    al_draw_tinted_scaled_rotated_bitmap_region(
        bmp.get(),
        srcX, 0,                 // region 左上
        frameW, frameH,         // region 尺寸
        al_map_rgba(255,255,255,255),
        frameW/2.0f, frameH/2.0f,  // 繪製時的原點（置中）
        Position.x, Position.y,    // 畫到場景的座標
        1,            // x 方向等比縮放
        1,            // y 方向等比縮放
        Rotation,
        0
    );

}