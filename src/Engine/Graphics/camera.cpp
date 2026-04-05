#include "Engine/Utils/matricies.hpp"
#include <Engine/Graphics/camera.hpp>
#include <cmath>

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
    return Matrix4x4::CreateOrthographicOffCenter (std::round (x),
                                                   std::round (x + w),
                                                   std::round (y + h),
                                                   std::round (y),
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