#pragma once
#include <algorithm>

namespace Collision { 
    bool circleRectangle(
        float cx, 
        float cy, 
        float cr, 
        float rx, 
        float ry, 
        float rw, 
        float rh
    );

    bool circleCircle(
        float c1x,
        float c1y,
        float c1r,
        float c2x,
        float c2y,
        float c2r
    );
}