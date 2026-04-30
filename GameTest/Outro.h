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

    float m_endTimer = 0.0f;
    static constexpr float END_DELAY = 3.0f;

    static constexpr int   SLIDE_COUNT = 7;
    static constexpr float SLIDE_DURATION = 2.0f;
    static constexpr float TYPE_SPEED = 0.03f;


    const char* m_lines[SLIDE_COUNT] = {
        "The watch pieces begin to fit together...",
        "A miraculous change on the tracks ahead...",
        "The conductor is stirring from his unnatural sleep...",
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