#pragma once
#include "app\app.h"
#include <string>

class Outro {
private:

    CSimpleSprite* m_slides[7];
    CSimpleSprite* m_dialogueBox;

    int   m_currentSlide;
    float m_slideTimer;

    std::string m_fullText;
    std::string m_displayedText;
    float m_typeTimer;
    int   m_typeIndex;

    bool  m_enterWasDown;
    bool  m_done;

    static constexpr int   SLIDE_COUNT = 7;
    static constexpr float TYPE_SPEED = 0.03f;


    const char* m_lines[SLIDE_COUNT] = {
        "All seven gears click into place!",
        "As promised, the clock wakes the conductor from his trance",
        "And the tracks are magically repaired!",
        "",
        "",
        "",
        ""
    };

    void StartSlide(int index);

public:
    Outro();
    ~Outro();

    void Update(float deltaTime);
    void Render();
    bool ShouldReturnToMenu() const;

    bool IsDone() const;
};