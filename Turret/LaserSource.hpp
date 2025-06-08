#ifndef LASERSOURCE_HPP
#define LASERSOURCE_HPP
#include "Turret.hpp"

class LaserSource : public Turret {
public:
    static const int Price;
    float towerdirection ;
    bool Adjustmode;
    LaserSource(float x, float y, float rotation);  
    void CreateBullet() override;
    void Update(float deltaTime) override;
};
#endif   // LASERTURRET_HPP
