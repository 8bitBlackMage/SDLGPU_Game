#include "Engine/Utils/matricies.hpp"
#include <Engine/Graphics/camera.hpp>

Camera::Camera (float initialW, float initialH) : x (0),
                                                  y (0),
                                                  w (initialW),
                                                  h (initialH),
                                                  zNearPlane (0),
                                                  zFarPlane (-1)
{
}

Matrix4x4 Camera::getCurrentMatrix()
{
    return Matrix4x4::CreateOrthographicOffCenter (x,
                                                   x + w,
                                                   y + h,
                                                   y,
                                                   zNearPlane,
                                                   zFarPlane);
}

void Camera::setX (float newX)
{
    x = newX;
}

void Camera::setY (float newY)
{
    y = newY;
}

float Camera::getX()
{
    return x;
}

float Camera::getY()
{
    return y;
}