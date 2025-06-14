#ifndef BEAM_HPP
#define BEAM_HPP
#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
    struct Point;
}   // namespace Engine

class Beam : public Bullet {
private:
    int     hitCount = 0;
    // 新增動畫控制參數
    float   animTimer    = 0.0f;
    int     animFrame    = 0;       // 0 ~ 4
    static constexpr int   FRAME_COUNT    = 4;
    static constexpr float FRAME_DURATION = 0.1f; // 每幀 0.1 秒
public:
    Turret* GetParent() const { return parent; }
    explicit Beam(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret *parent);
    void OnExplode(Enemy *enemy) override;
    void Update(float deltaTime) override ;
    void Draw() const override;
    
};
#endif   // LASERBULLET_HPP
