#pragma once
#include "app\app.h"
#include <string>

class BadOutro {
private:
    CSimpleSprite* m_slides[3];
    CSimpleSprite* m_dialogueBox;

    int   m_currentSlide;
    float m_slideTimer;
    float m_endTimer;

    std::string m_fullText;
    std::string m_displayedText;
    float m_typeTimer;
    int   m_typeIndex;

    static constexpr int   SLIDE_COUNT = 3;
    static constexpr float SLIDE_DURATION = 5.0f;
    static constexpr float TYPE_SPEED = 0.03f;
    static constexpr float END_DELAY = 3.0f;

    const char* m_lines[SLIDE_COUNT] = {
        "The gears were never recovered.",
        "Without the conductor, no one could stop the train.",
        "The passengers never reached their destination. The Goblin disappeared in a puff of foul-smelling smoke."
    };

    void StartSlide(int index);

public:
    BadOutro();
    ~BadOutro();

    void Update(float deltaTime);
    void Render();

    bool IsDone() const;
    bool ShouldReturnToMenu() const;
};