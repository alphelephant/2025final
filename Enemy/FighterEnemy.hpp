#ifndef FIGHTERENEMY_HPP
#define FIGHTERENEMY_HPP
#include "Enemy.hpp"
#include "Engine/Sprite.hpp"

class FighterEnemy : public Enemy {
private:
    Sprite head;
    float targetRotation;

public:
    FighterEnemy(int x, int y);
    void Draw() const override;
    void Update(float deltaTime) override;
};
#endif   // FIGHTERENEMY_HPP
