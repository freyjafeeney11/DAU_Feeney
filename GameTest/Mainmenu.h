#pragma once
#include "app\app.h"

class MainMenu {
private:
    CSimpleSprite* m_background;

public:
    MainMenu();
    ~MainMenu();

    void Update(float deltaTime);
    void Render();

    bool ShouldStart() const;
};