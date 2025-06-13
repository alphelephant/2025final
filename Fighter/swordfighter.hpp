#ifndef SWORDFIGHTER_HPP
#define SWORDFIGHTER_HPP
#include "Fighter.hpp"

class SwordFighter : public Fighter {
private:
    std::shared_ptr<ALLEGRO_BITMAP> runBmp;
    std::shared_ptr<ALLEGRO_BITMAP> attackBmp;
    // 每個動畫的 frame 數量
    int runFrameCount;
    int attackFrameCount;
    // 目前計時器
    float animTimer = 0;
    // 每幀持續時間（秒）
    float frameDuration = 0.1f;
    // 當前播放的 state
    bool isAttacking = false;
public:
    SwordFighter(float x, float y);
    void Update(float deltaTime) override;
    void Draw() const override;
    void AttackEnemy(Enemy *enemy) override ;
    static const int Price = 250;
};

#endif   // SWORDFIGHTER_HPP