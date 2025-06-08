#include <cmath>
#include <string>

#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Resources.hpp"
#include "ExplosionEffect.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Sprite.hpp"

PlayScene *ExplosionEffect::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
ExplosionEffect::ExplosionEffect(float x, float y) : Sprite("play/explosion-1.png", x, y), timeTicks(0) {
    for (int i = 1; i <= 5; i++) {
        bmps.push_back(Engine::Resources::GetInstance().GetBitmap("play/explosion-" + std::to_string(i) + ".png"));
    }
}
void ExplosionEffect::Update(float deltaTime) {
    timeTicks += deltaTime;
    if (timeTicks >= timeSpan) {
        getPlayScene()->EffectGroup->RemoveObject(objectIterator);
        return;
    }
    int phase = floor(timeTicks / timeSpan * bmps.size());
    bmp = bmps[phase];
    Sprite::Update(deltaTime);
}

PlayScene* ShockwaveEffect::getPlayScene() {
    return dynamic_cast<PlayScene*>(
        Engine::GameEngine::GetInstance().GetActiveScene());
}

ShockwaveEffect::ShockwaveEffect(float x, float y, float radius) : Sprite("play/shockwave.png", x, y),maxRadius(radius){
    auto bmp = Engine::Resources::GetInstance().GetBitmap("play/shockwave.png");
    int w = al_get_bitmap_width(bmp.get());
    startScale = (w / 2.0f);
    Anchor.x = 0.5f;
    Anchor.y = 0.5f;
    Size.x = 0;
    Size.y = 0;
    Tint = al_map_rgba(255, 255, 255, 255);
}

void ShockwaveEffect::Update(float deltaTime) {
    timeTicks += deltaTime;
    float t = timeTicks / timeSpan;
    if (t >= 1.0f) {
        getPlayScene()->EffectGroup->RemoveObject(objectIterator);
        return;
    }
    float currentRadius = maxRadius * t;
    Size.x = currentRadius*2.0f;
    Size.y = currentRadius *2.0f;
    this->Tint = al_map_rgba(255, 255, 255, static_cast<int>(255 * (1 - t)));

    Sprite::Update(deltaTime);
}
