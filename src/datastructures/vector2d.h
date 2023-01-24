#pragma once

#include "vector.h"

class Vector2d : public Vector
{
public:
    Vector2d(float x, float y);

private:
    float m_x, m_y;
};