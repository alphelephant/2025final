#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/Scoreboard.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"


void Scoreboard::Initialize() {         
    scoreLabels.clear();
    page = 0;
    ReadScore();

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    for (int i = 0; i < 6; i++) {
        Engine::Label* itemLabel = new Engine::Label("", "pirulen.ttf", 36, halfW, halfH - 120 + i * 60, 255, 10, 255, 255, 0.5, 0.5);
        AddNewObject(itemLabel);
        scoreLabels.push_back(itemLabel);
    }

    drawscore();

    AddNewObject(new Engine::Label("Score Board", "pirulen.ttf", 60, halfW, halfH / 3 + 50, 100, 255, 255, 255, 0.5, 0.5));

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 150, halfH * 3 / 2 + 80, 300, 75);
    btn->SetOnClickCallback(std::bind(&Scoreboard::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 36, halfW, halfH * 3 / 2 + 80 +37, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 500, halfH * 3 / 2 + 80, 300, 75);
    btn->SetOnClickCallback(std::bind(&Scoreboard::PrevPageOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 36, halfW - 350, halfH * 3 / 2 + 80 +37, 0, 0, 0, 255, 0.5, 0.5));

    // 下一頁按鈕
    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW + 200, halfH * 3 / 2 + 80, 300, 75);
    btn->SetOnClickCallback(std::bind(&Scoreboard::NextPageOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 36, halfW + 200 + 150, halfH * 3 / 2 + 80 +37, 0, 0, 0, 255, 0.5, 0.5));
}
void Scoreboard::Terminate() {
   /* AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();*/
    IScene::Terminate();

}
void Scoreboard::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void Scoreboard::ReadScore() {
    
    scorebs.clear(); 
    std::ifstream file("Resource/scoreboard.txt");
    //std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        int score;
        // Read the name first, then the score
        if (iss >> name >> score) {
            scorebs.push_back({name, score});
        }
    }
    file.close();   
    // Sort the scores in descending order
    std::sort(scorebs.begin(), scorebs.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second; // Sort by score in descending order
    }); 
}
void Scoreboard::drawscore() {

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    
    // 計算目前頁面要顯示的記錄範圍
    int start = page * 6;
    int end = std::min(static_cast<int>(scorebs.size()), start + 6);
    
    // 更新每個項目標籤的文字
    for (int i = 0; i < 6; i++) {
        if (start + i < end) {
            // 有資料，格式化顯示
            std::string rankText = std::to_string(start + i + 1);
            std::string nameText = scorebs[start + i].first;
            std::string scoreText = std::to_string(scorebs[start + i].second);
            
            // 確保對齊 (可以用空格做簡單對齊，或使用固定寬度格式)
            std::string formattedText = rankText + " " + nameText + " " + scoreText;            
            scoreLabels[i]->Text = formattedText; 
        } else {
            // 沒有資料，清空文字
            scoreLabels[i]->Text = "";
        }
    }
}
void Scoreboard::PrevPageOnClick() {
    if (page > 0) {
        page--; 
        drawscore();
    }
}
void Scoreboard::NextPageOnClick() {
    if ((page + 1) * 6 < scorebs.size()) { 
        page++; 
        drawscore(); 
    }
}

