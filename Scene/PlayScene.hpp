#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include "Turret/LaserSource.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
class Turret;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine

class PlayScene final : public Engine::IScene {
private:
    LaserSource* lastPlacedLaserSource = nullptr; 

    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;
    //for shovel mode
    bool shovelMode = false;
    Engine::Sprite* shovelPreview = nullptr;  // Shovel 的預覽圖像
    Turret* selectedTurret = nullptr;
protected:
    int lives;
    int money;
    int SpeedMult;
    int score = 0;
    int maxlives;
    float laserEyesTimer = 0.0f;
    bool angryMode = false; // Angry mode for laser eyes 
    int enemylives ;
    int maxenemylives ;

public:
    static bool DebugMode;
    static const std::vector<Engine::Point> directions;
    static const int MapWidth, MapHeight;
    static const int BlockSize;
    static const int Sandpos = 1280;
    static const float DangerTime;
    static const Engine::Point SpawnGridPoint;
    static const Engine::Point EndGridPoint;
    static const std::vector<int> code;
    int MapId;
    float ticks;
    float deathCountDown;
    // Map tiles.
    enum TileType {
        TILE_DIRT,
        TILE_FLOOR,
        TILE_OCCUPIED,
        TILE_HOLE,
        TILE_UPWALL,
        TILE_DOWNWALL,
        TILE_LEFTWALL,
        TILE_RIGHTWALL,
        TILE_DOOR,
    };
    Group *TileMapGroup;
    Group *GroundEffectGroup;
    Group *DebugIndicatorGroup;
    Group *BulletGroup;
    Group *TowerGroup;
    Group *EnemyGroup;
    Group *EffectGroup;
    Group *UIGroup;
    Group *FighterGroup;
    Group *EnemyBulletGroup;
    Group *MissileGroup;
    
    Engine::Label *UIMoney;
    Engine::Label *UILives;
    Engine::Image *imgTarget;
    Engine::Sprite *dangerIndicator;
    Turret *preview;
    std::vector<std::vector<TileType>> mapState;
    std::vector<std::vector<int>> mapDistance;
    std::vector<std::vector<int>> FightDistance;
    std::list<std::pair<int, float>> enemyWaveData;
    std::list<int> keyStrokes;
    int GetScore() const { return score; }
    static Engine::Point GetClientSize();
    explicit PlayScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void Hit();
    void Hitenemy();
    int GetMoney() const;
    void EarnMoney(int money);
    void ReadMap();
    void ReadEnemyWave();
    void ConstructUI();
    void SetShovelMode(bool enabled);
    void UIBtnClicked(int id);
    bool CheckSpaceValid(int x, int y);
    std::vector<std::vector<int>> CalculateBFSDistance();
    std::vector<std::vector<int>> CalculateDistance(int x, int y, int endx, int endy);
    // void ModifyReadMapTiles();
    void BacktoStageSelect();
};
#endif   // PLAYSCENE_HPP
