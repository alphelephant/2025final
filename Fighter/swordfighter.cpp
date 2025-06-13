#include <string>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <allegro5/color.h>
#include "Swordfighter.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Image.hpp"

// TODO HACKATHON-3 (1/3): You can imitate the 2 files: 'SoldierEnemy.hpp', 'SoldierEnemy.cpp' to create a new enemy.
SwordFighter::SwordFighter(float x, float y) : Fighter("play/Swordman.png", x, y,
    /*detactradian*/25, /*speed*/250,
    /*hp*/ 100,
    /*attack range*/ 100 ,/*damageRange*/100, /*attackspeed*/ 0.5f),runFrameCount(16),attackFrameCount(7)    
    {
    damage = 50;
    runBmp = Engine::Resources::GetInstance().GetBitmap("play/RUN.png");
    attackBmp = Engine::Resources::GetInstance().GetBitmap("play/ATTACK 1.png");
    Anchor.x = 0.5f; // 設定錨點為圖片中心
    Anchor.y = 0.5f; // 設定錨點為圖片中心
    isSwordFighter = true;
}
void SwordFighter::Update(float deltaTime) {
    Fighter::Update(deltaTime);
    // 判斷有沒有在攻擊範圍內、reload 是否歸零來切換 state
    if (TargetEnemy && reload <= attackSpeed - frameDuration) {
        isAttacking = true;
    } else if (!TargetEnemy) {
        isAttacking = false;
    }
    animTimer += deltaTime;
    // 迴圈播放
    float cycle = (isAttacking ? attackFrameCount : runFrameCount) * frameDuration;
    if (animTimer >= cycle) animTimer -= cycle;
}
void SwordFighter::AttackEnemy(Enemy *enemy) {
    Fighter::AttackEnemy(enemy);
    AudioHelper::PlayAudio("sword.wav");

}
void SwordFighter::Draw() const {
    std::shared_ptr<ALLEGRO_BITMAP> bmp = isAttacking ? attackBmp : runBmp;
    if (!bmp) {
        printf("SwordFighter::Draw: bmp is null\n");
        // 如果沒有載入圖片，則不繪製
        return;
    }
    int   totalFrames   = isAttacking ? attackFrameCount : runFrameCount;
    float bmpW          = al_get_bitmap_width(bmp.get());
    float bmpH          = al_get_bitmap_height(bmp.get());
    float frameW        = bmpW / totalFrames;
    float frameH        = bmpH;
    int   idx           = int(animTimer / frameDuration) % totalFrames;
    float sx            = idx * frameW;
    int flag  = 0;
if (std::fabs(Rotation - ALLEGRO_PI) < 0.1)
    flag = ALLEGRO_FLIP_VERTICAL;
    // draw region: al_draw_tinted_scaled_rotated_bitmap_region
    al_draw_tinted_scaled_rotated_bitmap_region(
        bmp.get(), 
        sx, 0,                   // src x, y
        frameW, frameH,
        Tint,                     // src w, h
        frameW * Anchor.x  ,      // cx
        frameH * Anchor.y  ,      // cy
        Position.x, Position.y, // dx, dy
        2 ,        // xscale
        2 ,        // yscale
        Rotation, flag             // angle, flags
    );
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
    // 最後再畫血條、Debug 圈
    //Fighter::Draw();
}
