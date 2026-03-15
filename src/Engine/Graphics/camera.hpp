#pragma once
#include "Engine/Utils/matricies.hpp"
class Camera
{
public:
    Camera (float initialW, float initialH);
    Matrix4x4 getCurrentMatrix();
    void setX (float newX);
    void setY (float newY);
    float getX();
    float getY();

private:
    float x, y, w, h;
    float zNearPlane, zFarPlane;
};