#pragma once
#include "app\app.h"
#include <string>

class Intro {
private:
    CSimpleSprite* m_hypnotize;
    CSimpleSprite* m_slides[3];
    CSimpleSprite* m_dialogueBox;

    int   m_currentSlide;
    float m_slideTimer;
    bool  m_hypnotizeDone;

    // Typewriter
    std::string m_fullText;
    std::string m_displayedText;
    float m_typeTimer;
    int   m_typeIndex;

    static constexpr float SLIDE_DURATION = 4.5f;
    static constexpr float TYPE_SPEED = 0.015f;

    const char* m_lines[4] = {
        "Welcome aboard young one.. I have hypnotized the conductor with my special clock!",
        "We are now rolling towards a very unfortunate and permanent fate...",
        "And look.. my clock has broken into seven pieces!",
        "To save everyone, recover the gears.. by picking pockets and bringing me gossip!!"
    };

    void StartSlide(int index);

public:
    Intro();
    ~Intro();

    void Update(float deltaTime);
    void Render();

    bool IsDone() const;
};