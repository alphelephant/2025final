#include <functional>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"

void WinScene::Initialize() {
    namesaved = false;
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    auto play = dynamic_cast<PlayScene*>(
    Engine::GameEngine::GetInstance().GetScene("play"));    
    if (play) score = play->GetScore();
    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH-80, 350, 350, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));
    itemLabel = new Engine::Label("", "pirulen.ttf", 36, halfW, halfH * 11/8 + 25 , 255, 10, 255, 255, 0.5, 0.5);
    AddNewObject(itemLabel);
    itemLabel->Text = "enteryourname";

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayAudio("win.wav");
}
void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }
}
void WinScene::BackOnClick(int stage) {
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void WinScene::OnKeyDown(int keyCode) {
    if (namesaved) {
        return;
    }
    IScene::OnKeyDown(keyCode);
    if(keyCode == ALLEGRO_KEY_ENTER) {
        if (nameList.empty()) {
            itemLabel->Text = "Name cannot be empty!";
            return;
        }
        std::string name;
        for (auto it = nameList.begin(); it != nameList.end(); ++it) {
            name += *it;
        }
        std::ofstream file("Resource/scoreboard.txt", std::ios::app);
        std::ofstream file2("../Resource/scoreboard.txt", std::ios::app);
        if (!file.is_open()) {
            itemLabel->Text = "Failed to save name!";
            return;
        }
        if (!file2.is_open()) {
            itemLabel->Text = "Failed to open f2!";
        }
        file << name << " " << score << "\n";
        file2 << name << " " << score << "\n";
        itemLabel->Text = name + " " + std::to_string(score);
        file.flush();//有點問題的東西 
        file2.flush();
        file.close();
        file2.close();
        namesaved = true;
    }
    else{
        char c = 0;
        if(keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z) {
            c = keyCode - ALLEGRO_KEY_A + 'A';
        }
        else if(keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
            c = keyCode - ALLEGRO_KEY_0 + '0';
        }
        else if(keyCode == ALLEGRO_KEY_SPACE) {
            itemLabel->Text = "notallowed";
            return;
        }
        else if(keyCode == ALLEGRO_KEY_BACKSPACE) {
            if (!nameList.empty()) {
                nameList.pop_back();
            }
        }
        if(c != 0){
            nameList.push_back(c);
            if (nameList.size() > 10) {
                nameList.pop_front();
            }
        }

        std::string name = "";
        for (auto it = nameList.begin(); it != nameList.end(); ++it) {
            name += *it;
        }
        if(name.empty()) {
            itemLabel->Text = "enteryourname";
        }
        else{
            itemLabel->Text = name;
        }
        
    }

}
