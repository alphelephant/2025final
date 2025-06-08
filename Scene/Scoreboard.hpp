 #ifndef Scoreboard_HPP
#define Scoreboard_HPP
#include <memory>

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class Scoreboard final : public Engine::IScene {
private:
   // std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
   std::vector<std::string> lines;
   std::vector<std::pair<std::string, int>> scorebs;
   std::vector<Engine::Label *> scoreLabels;
   int page = 0;

public:
    explicit Scoreboard() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
    void ReadScore();
    void drawscore();
    void PrevPageOnClick();
    void NextPageOnClick();
    void ClearScoreLabels();
    //void BGMSlideOnValueChanged(float value);
    //void SFXSlideOnValueChanged(float value);
};

#endif   // SettingsScene_HPP
