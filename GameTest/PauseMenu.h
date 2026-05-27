#pragma once
#include "app\app.h"
#include <stdlib.h>

class PauseMenu {
private:
    int m_choice;
    bool m_navDown;
    bool m_enterDown;
    bool m_showControlsTab;
    CSimpleSprite* m_background;
    CSimpleSprite* m_dim;
    CSimpleSprite* m_text;
    CSimpleSprite* m_controlsBg;

public:
    PauseMenu();
    ~PauseMenu();
    void Update(bool& isPaused);
    void Render();

    bool IsControlsTabOpen() const { return m_showControlsTab; }
    void CloseControlsTab() { m_showControlsTab = false; }
};