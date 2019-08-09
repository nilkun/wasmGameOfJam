#include "./Collision.hpp"

namespace Collision {
    bool circleRectangle(
            float cx, 
            float cy, 
            float cr, 
            float rx, 
            float ry, 
            float rw, 
            float rh
    ) {
        // NON ROTATED RECTANGLE
        const float deltaX = cx - std::max(rx, std::min(cx, rx + rw));
        const float deltaY = cy - std::max(ry, std::min(cy, ry + rh));
        return ((deltaX * deltaX + deltaY * deltaY) < cr * cr);
    };
    bool circleCircle(
        float c1x,
        float c1y,
        float c1r,
        float c2x,
        float c2y,
        float c2r = 0
    ) {
        float a = c1x - c2x;
        float b = c1y - c2y;
        float r = c1r + c2r;
        return a * a + b * b < r * r;
    }
};