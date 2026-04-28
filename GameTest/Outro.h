#pragma once
#include "app\app.h"
#include <string>

class Outro {
private:

    CSimpleSprite* m_slides[4];
    CSimpleSprite* m_dialogueBox;

    int   m_currentSlide;
    float m_slideTimer;

    std::string m_fullText;
    std::string m_displayedText;
    float m_typeTimer;
    int   m_typeIndex;

    float m_endTimer = 0.0f;
    static constexpr float END_DELAY = 3.0f;

    static constexpr int   SLIDE_COUNT = 4;
    static constexpr float SLIDE_DURATION = 5.0f;
    static constexpr float TYPE_SPEED = 0.03f;


    const char* m_lines[SLIDE_COUNT] = {
        "You've done it... all the missing gears, recovered at last.",
        "The pocket watch stirs... and begins to tick once more.",
        "The conductor's eyes flutter open, confused but alive.",
        "With a great screech of brakes, the train shudders to a halt. Everyone is safe."
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