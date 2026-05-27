
#pragma once
#include "app\app.h"

namespace UserControls {
    inline bool IsUserGoingRight() {
        return App::IsKeyPressed(VK_RIGHT);
    }
    inline bool IsUserGoingLeft() {
        return App::IsKeyPressed(VK_LEFT);
    }
    inline bool IsUserGoingUp() {
        return App::IsKeyPressed(VK_UP);
    }
    inline bool IsUserGoingDown() {
        return App::IsKeyPressed(VK_DOWN);
    }
    inline bool IsUserSprinting() {
        return App::IsKeyPressed(VK_SHIFT);
    }
}