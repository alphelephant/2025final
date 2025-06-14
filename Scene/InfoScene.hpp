#ifndef INFOSCENE_HPP
#define INFOSCENE_HPP
#include "Engine/IScene.hpp"
#include <vector>
#include <list>
#include <string>
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"

class InfoScene final : public Engine::IScene {
private:
    struct InfoItem {
        std::string imagePath;
        std::string name;
        int hp;
        int damage;
        int dps;
        int speed_range;
    };
    std::vector<InfoItem> enemyItems;
    std::vector<InfoItem> turretItems;
    std::vector<Engine::ImageButton*> currentItemButtons;
    Engine::Label* descLabel = nullptr;
    /*Engine::Label* hpLabel = nullptr;
    Engine::Label* damageLabel = nullptr;
    Engine::Label* speedLabel = nullptr;*/
    int selectedType = 0; // 0: enemy, 1: turret

public:
    explicit InfoScene() = default;
    void Initialize() override;
    void Terminate() override;
    void OnEnemyClick(int idx);
    void OnTurretClick(int idx);
    void ShowEnemy();
    void ShowTurret();
};

#endif   // INFOSCENE_HPP