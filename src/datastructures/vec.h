#pragma once

struct Vec2
{
    double x, y;
};

struct Vec3
{
    double x, y, z;
    double operator [](int idx) const {
        return 0;
    }
};