#ifndef EXPLOSIONEFFECT_HPP
#define EXPLOSIONEFFECT_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <vector>

#include "Engine/Sprite.hpp"

class PlayScene;

class ExplosionEffect : public Engine::Sprite {
protected:
    PlayScene *getPlayScene();
    float timeTicks;
    std::vector<std::shared_ptr<ALLEGRO_BITMAP>> bmps;
    float timeSpan = 0.5;

public:
    ExplosionEffect(float x, float y);
    void Update(float deltaTime) override;
};

class ShockwaveEffect : public Engine::Sprite {
protected:
    PlayScene *getPlayScene();
    float   timeTicks  = 0.0;
    float   timeSpan   = 0.4;    
    float   maxRadius;            
    float   startScale;            
public:
    ShockwaveEffect(float x, float y, float radius);
    void Update(float deltaTime) override;
};
#endif   // EXPLOSIONEFFECT_HPP
