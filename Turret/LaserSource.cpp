#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Bullet/Beam.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "LaserSource.hpp"
#include "Scene/PlayScene.hpp"

const int LaserSource::Price = 500;
LaserSource::LaserSource(float x, float y, float rotation) : Turret("play/tower-base.png", "play/turret-7.png", x, y, 450, Price, 2.0f) {
    Anchor.y += 8.0f / GetBitmapHeight();
    Rotation = rotation;
}
void LaserSource::CreateBullet() {
    auto scene = getPlayScene();
    
    /*for (auto &obj : scene->BulletGroup->GetObjects()) {
        auto beam = dynamic_cast<Beam*>(obj);
        if (beam && beam->GetParent() == this) {
            scene->BulletGroup->RemoveObject(beam->GetObjectIterator());
            break;
        }
    }
    if (BeamCoolDown) return;*/
    Engine::Point diff = Engine::Point(cos(towerdirection - ALLEGRO_PI / 2), sin(towerdirection - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    Engine::Point normal = Engine::Point(-normalized.y, normalized.x);
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new Beam(Position + normalized * 230, diff, rotation, this));
    //getPlayScene()->BulletGroup->AddNewObject(new Beam(Position + normalized * 36 + normal * 6, diff, rotation, this));
    AudioHelper::PlayAudio("laser.wav");
}
void LaserSource::Update(float deltaTime) {
    if (Adjustmode) {
        Engine::Point mousePos = Engine::GameEngine::GetInstance().GetMousePosition();
        towerdirection = atan2(mousePos.y - Position.y, mousePos.x - Position.x) + 1.57079632679;
        Rotation = towerdirection;
    }
    else{
        Locked = true;
        Turret::Update(deltaTime);
    }  
}