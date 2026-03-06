// UserControls.h
#pragma once
#include "app\app.h"

namespace UserControls {
    inline bool IsUserGoingRight() {
        return App::GetController().GetLeftThumbStickX() > 0.5f;
    }
    inline bool IsUserGoingLeft() {
        return App::GetController().GetLeftThumbStickX() < -0.5f;
    }
    inline bool IsUserGoingUp() {
        return App::GetController().GetLeftThumbStickY() > 0.5f;
    }
    inline bool IsUserGoingDown() {
        return App::GetController().GetLeftThumbStickY() < -0.5f;
    }
    inline bool IsUserSprinting() {
        return App::IsKeyPressed(VK_SHIFT);
    }
}