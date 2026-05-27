#pragma once
#include "app\app.h"
#include <string>

class Intro {
private:
    CSimpleSprite* m_hypnotize;
    CSimpleSprite* m_slides[3];
    CSimpleSprite* m_dialogueBox;

    int   m_textIndex;
    bool  m_enterDown;

    std::string m_fullText;
    std::string m_displayedText;
    float m_typeTimer;
    int   m_typeIndex;

    static constexpr float TYPE_SPEED = 0.015f;

    const char* m_lines[5] = {
        "Welcome aboard young one.. I have hypnotized the conductor with my special clock!",
        "We are now rolling towards a very unfortunate and permanent fate...",
        "And look.. my clock has broken into seven pieces!",
        "To save everyone, recover ALL SEVEN GEARS..",
        "..by picking pockets and bringing me gossip!!"
    };

    void StartLine(int index);

public:
    Intro();
    ~Intro();

    void Update(float deltaTime);
    void Render();

    bool IsDone() const;
};