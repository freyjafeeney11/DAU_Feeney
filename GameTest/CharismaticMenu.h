#pragma once
#include "app\app.h"
#include <string>

class CharismaticMenu {
private:
    CSimpleSprite* m_background;
    CSimpleSprite* m_dim;

    std::string m_fullText;
    std::string m_displayedText;
    float m_typeTimer;
    int   m_typeIndex;
    bool  m_enterDown;
    bool  m_dismissed;

    static constexpr float TYPE_SPEED = 0.04f;

public:
    CharismaticMenu();
    ~CharismaticMenu();

    void Show(int livesRemaining);
    void Update(bool& outDismissed);
    void Render();

    bool IsDismissed() const { return m_dismissed; }
};
