#pragma once

#include "Graphics/Pipelines/lineRenderer.hpp"
#include <glm/vec2.hpp>
class Line
{
public:
    Line (float x1, float y1, float x2, float y2);
    friend class LineRenderer;

private:
    glm::vec2 start;
    glm::vec2 end;
};