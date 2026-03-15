#include <Engine/Utils/matricies.hpp>

Matrix4x4 Matrix4x4::CreateOrthographicOffCenter (
    float left,
    float right,
    float bottom,
    float top,
    float zNearPlane,
    float zFarPlane)
{
    return (Matrix4x4) {
        2.0f / (right - left),
        0,
        0,
        0,
        0,
        2.0f / (top - bottom),
        0,
        0,
        0,
        0,
        1.0f / (zNearPlane - zFarPlane),
        0,
        (left + right) / (left - right),
        (top + bottom) / (bottom - top),
        zNearPlane / (zNearPlane - zFarPlane),
        1
    };
}