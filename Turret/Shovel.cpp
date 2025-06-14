#include <allegro5/color.h>

#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Shovel.hpp"

PlayScene *Shovel::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}
Shovel::Shovel(std::string img, std::string imgIn, Engine::Sprite Base, Engine::Sprite shovel, float x, float y, int money) 
  : ImageButton(img, imgIn, x, y), money(0), Base(Base), shovel(shovel) {
}
void Shovel::Update(float deltaTime) {
    ImageButton::Update(deltaTime);
    if (getPlayScene()->GetMoney() >= money) {
        Enabled = true;
        Base.Tint = shovel.Tint = al_map_rgba(255, 255, 255, 255);
    } else {
        Enabled = false;
        Base.Tint = shovel.Tint = al_map_rgba(0, 0, 0, 160);
    }

}
void Shovel::Draw() const {
    ImageButton::Draw();
    Base.Draw();
    shovel.Draw();
}