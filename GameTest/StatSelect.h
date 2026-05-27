#pragma once
#include "app\app.h"
#include <string>

class StatSelect {
private:
    CSimpleSprite* m_bg;
    CSimpleSprite* m_uiScreen;
    CSimpleSprite* m_dialogueBg;

    int   m_choice;
    bool  m_navDown;
    bool  m_enterDown;
    bool  m_selected;

    float m_fadeTimer;
    float m_dialogueTimer;
    bool  m_fading;
    bool  m_showDialogue;
    bool  m_done;

    std::string m_fullText;
    std::string m_displayedText;
    float m_typeTimer;
    int   m_typeIndex;

    static constexpr float FADE_DURATION     = 1.2f;
    static constexpr float DIALOGUE_DURATION = 4.5f;
    static constexpr float TYPE_SPEED        = 0.022f;

    const char* m_names[3]  = { "Charismatic", "Dexterous", "Lucky" };

public:
    StatSelect();
    ~StatSelect();

    void Update(float deltaTime);
    void Render();

    bool IsDone() const { return m_done; }
};
