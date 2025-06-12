#ifndef SWORDFIGHTER_HPP
#define SWORDFIGHTER_HPP
#include "Fighter.hpp"

class SwordFighter : public Fighter {
public:
    SwordFighter(float x, float y);
    void Draw() const override {
        // Draw the sword fighter with a specific image.
        Engine::Sprite::Draw();

    }
};

#endif   // SWORDFIGHTER_HPP