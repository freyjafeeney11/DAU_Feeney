#pragma once
#include "app\app.h"

class MainMenu {
private:
    CSimpleSprite* m_background;
    CSimpleSprite* m_cursor;
    int m_choice;
    bool m_navDown;
    bool m_startSelected;
    bool m_exitSelected;

public:
    MainMenu();
    ~MainMenu();

    void Update(float deltaTime);
    void Render();

    bool ShouldStart() const { return m_startSelected; }
    bool ShouldExit() const { return m_exitSelected; }
};