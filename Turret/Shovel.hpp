#ifndef SHOVEL_HPP
#define SHOVEL_HPP
#include <string>

#include "Engine/Sprite.hpp"
#include "UI/Component/ImageButton.hpp"

class PlayScene;

class Shovel : public Engine::ImageButton {
protected:
    PlayScene *getPlayScene();

public:
    int money;
    Engine::Sprite Base;
    Engine::Sprite shovel;
    Shovel(std::string img, std::string imgIn, Engine::Sprite Base, Engine::Sprite shovel, float x, float y, int money);
    void Update(float deltaTime) override;
    void Draw() const override;
};
#endif 