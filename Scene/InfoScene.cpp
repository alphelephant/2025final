#include "InfoScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Engine/GameEngine.hpp"
#include "Enemy/Enemy.hpp"
#include <functional>
#include "Enemy/Enemy.hpp"

void InfoScene::Initialize() {
    // 填入敵人資料
    enemyItems = {
        {"play/enemy-1.png", "Soldier", "a"},
        {"play/enemy-2.png", "Hawk-X", "b"},
        {"play/enemy-3.png", "Guardian", "c"},
        {"play/enemy-6.png", "Boomer", "d"},
        {"play/enemy-9.png", "Pulsefiend", "e"},
        {"play/enemy-10.png", "Sniper", "f"},
        {"play/enemy-7.png", "Destroyer", "g"}
    };
    // 填入砲台資料
    turretItems = {
        {"play/turret-1.png", "Gun", "1"},
        {"play/turret-2.png", "Cannon", "2"},
        {"play/turret-7.png", "Laser", "3"},
        {"play/enemy-4.png", "Fighter", "4"},
        
    };

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    // 背景
    AddNewObject(new Engine::Image("stage-select/background.png", 0, 0, w, h));
    // 標題
    AddNewObject(new Engine::Label("Info", "pirulen.ttf", 60, w/2, h/6 + 50, 100, 255, 255, 255, 0.5, 0.5));

    // 中間說明區背景（半透明）
    int infoBoxW = 500, infoBoxH = 320;
    int infoBoxX = w/2 - infoBoxW/2;
    int infoBoxY = 300;
    AddNewObject(new Engine::Image("stage-select/floor.png", infoBoxX, infoBoxY, infoBoxW, infoBoxH, 0, 0));
    
    // 中間介紹文字
    descLabel = new Engine::Label("Click The Image", "pirulen.ttf", 36, w/2, infoBoxY + 50, 0, 0, 0, 255, 0.5, 0);
    AddNewObject(descLabel);
    hpLabel = new Engine::Label("", "pirulen.ttf", 36, w/2, infoBoxY + 100, 0, 0, 0, 255, 0.5, 0);
    AddNewObject(hpLabel);
    damageLabel = new Engine::Label("", "pirulen.ttf", 36, w/2, infoBoxY + 150, 0, 0, 0, 255, 0.5, 0);
    speedLabel = new Engine::Label("", "pirulen.ttf", 36, w/2, infoBoxY + 200, 0, 0, 0, 255, 0.5, 0);
    AddNewObject(damageLabel);
    AddNewObject(speedLabel);

    // 返回按鈕
    auto backBtn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w - 250, h - 120, 200, 80);
    backBtn->SetOnClickCallback([]() {
        Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    });
    AddNewControlObject(backBtn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 36, w - 150, h - 80, 0, 0, 0, 255, 0.5, 0.5));

    // 顯示敵人與砲台
    ShowEnemy();
    ShowTurret();
}

void InfoScene::ShowEnemy() {
    // 左側敵人圖片
    int leftX = 100;
    int baseY = 50;
    int spacing = 120;
    int imgW = 100, imgH = 100;

    for (size_t i = 0; i < enemyItems.size(); ++i) {
        auto btn = new Engine::ImageButton(
            enemyItems[i].imagePath, enemyItems[i].imagePath, leftX, baseY + i * spacing, imgW, imgH);
        btn->SetOnClickCallback([this, i]() { OnEnemyClick(i); });
        AddNewControlObject(btn);
        currentItemButtons.push_back(btn);

        if (enemyItems[i].imagePath == "play/enemy-3.png") {
            // 在主體右側顯示頭像
            int headOffsetX = 20; // 與主體間距
            AddNewObject(new Engine::Image("play/enemy-3-head.png", leftX + headOffsetX, baseY + i * spacing, imgW, imgH));
        }
    }
}
void InfoScene::ShowTurret() {
    // 右側砲台圖片
    int rightX = Engine::GameEngine::GetInstance().GetScreenSize().x - 200;
    int baseY = 150;
    int spacing = 120;
    int imgW = 100, imgH = 100;

    for (size_t i = 0; i < turretItems.size(); ++i) {
        auto btn = new Engine::ImageButton(
            turretItems[i].imagePath, turretItems[i].imagePath, rightX, baseY + i * spacing, imgW, imgH);
        btn->SetOnClickCallback([this, i]() { OnTurretClick(i); });
        AddNewControlObject(btn);
        currentItemButtons.push_back(btn);
    }
}

void InfoScene::OnEnemyClick(int idx) {
    if (descLabel && idx < enemyItems.size()) {
        descLabel->Text = enemyItems[idx].name + " " + enemyItems[idx].description;
    }
    /*if (hpLabel && idx < enemyItems.size()) {
        hpLabel->Text = "HP: " + std::to_string(enemyItems[idx].hp);
    }
    if (damageLabel && idx < enemyItems.size()) {
        damageLabel->Text = "Damage: " + std::to_string(enemyItems[idx].damage);
    }
    if (speedLabel && idx < enemyItems.size()) {
        speedLabel->Text = "Speed: " + std::to_string(Enemy::GetInfo("speed"));
    }*/
}

void InfoScene::OnTurretClick(int idx) {
    if (descLabel && idx < turretItems.size()) {
        descLabel->Text = turretItems[idx].name + " " + turretItems[idx].description;
    }
}

void InfoScene::Terminate() {
    currentItemButtons.clear();
    IScene::Terminate();
}