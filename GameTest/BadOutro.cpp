#include "stdafx.h"
#include "BadOutro.h"

BadOutro::BadOutro() {
    m_slides[0] = App::CreateSprite(".\\TestData\\train_fall_1.png", 1, 1);
    m_slides[1] = App::CreateSprite(".\\TestData\\train_fall_2.png", 1, 1);
    m_slides[2] = App::CreateSprite(".\\TestData\\train_fall_3.png", 1, 1);

    for (int i = 0; i < SLIDE_COUNT; i++) {
        m_slides[i]->SetPosition(512.0f, 400.0f);
        m_slides[i]->SetScale(0.6f);
    }

    m_dialogueBox = App::CreateSprite(".\\TestData\\dialogue.png", 1, 1);
    m_dialogueBox->SetPosition(500.0f, 980.0f);
    m_dialogueBox->SetScale(0.6f);

    m_currentSlide = 0;
    m_slideTimer = 0.0f;
    m_endTimer = 0.0f;
    StartSlide(0);
}

BadOutro::~BadOutro() {
    for (int i = 0; i < SLIDE_COUNT; i++) delete m_slides[i];
    delete m_dialogueBox;
}

void BadOutro::StartSlide(int index) {
    if (index < SLIDE_COUNT) {
        m_fullText = m_lines[index];
        m_displayedText = "";
        m_typeTimer = 0.0f;
        m_typeIndex = 0;
    }
}

void BadOutro::Update(float deltaTime) {
    float dt = deltaTime / 1000.0f;
    m_slideTimer += dt;

    if (m_typeIndex < (int)m_fullText.size()) {
        m_typeTimer += dt;
        if (m_typeTimer >= TYPE_SPEED) {
            m_typeTimer = 0.0f;
            m_displayedText += m_fullText[m_typeIndex];
            m_typeIndex++;
        }
    }

    if (m_slideTimer >= SLIDE_DURATION && m_currentSlide < SLIDE_COUNT - 1) {
        m_slideTimer = 0.0f;
        m_currentSlide++;
        StartSlide(m_currentSlide);
    }

    if (IsDone()) m_endTimer += dt;
}

void BadOutro::Render() {
    if (m_currentSlide < SLIDE_COUNT) {
        m_slides[m_currentSlide]->Draw();
    }
    m_dialogueBox->Draw();
    App::Print(169, 710, m_displayedText.c_str(), 1.0f, 1.0f, 1.0f);
}

bool BadOutro::IsDone() const {
    return m_currentSlide >= SLIDE_COUNT - 1 && m_slideTimer >= SLIDE_DURATION;
}

bool BadOutro::ShouldReturnToMenu() const {
    return IsDone() && m_endTimer >= END_DELAY;
}