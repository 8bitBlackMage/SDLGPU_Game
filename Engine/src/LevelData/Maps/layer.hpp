#pragma once

#include <cstddef>

class Scene;

class Layer
{
public:
    size_t getuuid();

protected:
    Scene* owner;
    size_t uuid;
};