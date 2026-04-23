#pragma once
#include "app\app.h"
#include <stdlib.h>

class PauseMenu {
private:
    int m_choice;
    bool m_navDown;
    bool m_enterDown;
    CSimpleSprite* m_background;
    CSimpleSprite* m_dim;
    CSimpleSprite* m_text;

public:
    PauseMenu();
    ~PauseMenu();
    void Update(bool& isPaused);
    void Render();
};