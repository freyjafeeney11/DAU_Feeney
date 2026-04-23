#pragma once
#include "app\app.h"

class CaughtMenu {
private:
    CSimpleSprite* m_background;
    CSimpleSprite* m_dim;
    int  m_choice;
    bool m_navDown;
    bool m_enterDown;

public:
    CaughtMenu();
    ~CaughtMenu();

    void Update(bool& outRetry);
    void Render();
};