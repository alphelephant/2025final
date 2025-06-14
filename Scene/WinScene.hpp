#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class WinScene final : public Engine::IScene {
private:
    float ticks;
    bool namesaved ;
    ALLEGRO_SAMPLE_ID bgmId;

public:
    std::list<int> nameList;
    Engine::Label* itemLabel;
    explicit WinScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void OnKeyDown(int keyCode) override;
};

#endif   // WINSCENE_HPP
