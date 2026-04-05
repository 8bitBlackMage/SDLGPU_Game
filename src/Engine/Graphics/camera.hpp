#pragma once
#include <glm/mat4x4.hpp>
class Camera
{
public:
    Camera (float initialW, float initialH);
    glm::mat4x4 getCurrentMatrix();
    void setX (float newX);
    void setY (float newY);
    float getX();
    float getY();

private:
    float x, y, w, h;
    float zNearPlane, zFarPlane;
};