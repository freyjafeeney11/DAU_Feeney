#include "stdafx.h"
#include "Intro.h"

Intro::Intro() {
    m_hypnotize = App::CreateSprite(".\\TestData\\hypnotize.png", 4, 1);
    m_hypnotize->SetPosition(512.0f, 400.0f);
    m_hypnotize->SetScale(0.6f);
    m_hypnotize->CreateAnimation(0, 0.5f, { 0, 1, 2, 3 });
    m_hypnotize->SetAnimation(0);

    m_slides[0] = App::CreateSprite(".\\TestData\\train_fall_1.png", 1, 1);
    m_slides[1] = App::CreateSprite(".\\TestData\\train_fall_2.png", 1, 1);
    m_slides[2] = App::CreateSprite(".\\TestData\\train_fall_3.png", 1, 1);

    for (int i = 0; i < 3; i++) {
        m_slides[i]->SetPosition(512.0f, 400.0f);
        m_slides[i]->SetScale(0.6f);
    }

    m_dialogueBox = App::CreateSprite(".\\TestData\\dialogue.png", 1, 1);
    m_dialogueBox->SetPosition(512.0f, 680.0f);
    m_dialogueBox->SetScale(0.6f);

    m_currentSlide = 0;
    m_slideTimer = 0.0f;
    m_hypnotizeDone = false;

    StartSlide(0);
}

Intro::~Intro() {
    delete m_hypnotize;
    for (int i = 0; i < 3; i++) {
        delete m_slides[i];
    }
    delete m_dialogueBox;
}

void Intro::StartSlide(int index) {
    if (index < 4) {
        m_fullText = m_lines[index];
        m_displayedText = "";
        m_typeTimer = 0.0f;
        m_typeIndex = 0;
    }
}

void Intro::Update(float deltaTime) {
    m_slideTimer += deltaTime / 1000.0f;

    // Typewriter update
    if (m_typeIndex < (int)m_fullText.size()) {
        m_typeTimer += deltaTime / 1000.0f;
        if (m_typeTimer >= TYPE_SPEED) {
            m_typeTimer = 0.0f;
            m_displayedText += m_fullText[m_typeIndex];
            m_typeIndex++;
        }
    }

    if (!m_hypnotizeDone) {
        m_hypnotize->Update(deltaTime);
        if (m_slideTimer >= SLIDE_DURATION) {
            m_hypnotizeDone = true;
            m_slideTimer = 0.0f;
            StartSlide(1);
        }
        return;
    }

    if (m_slideTimer >= SLIDE_DURATION) {
        m_slideTimer = 0.0f;
        m_currentSlide++;
        if (m_currentSlide < 3) {
            StartSlide(m_currentSlide + 1);
        }
    }
}

void Intro::Render() {
    if (!m_hypnotizeDone) {
        m_hypnotize->Draw();
    }
    else if (m_currentSlide < 3) {
        m_slides[m_currentSlide]->Draw();
    }

    // Dialogue box and typewriter text at top of screen
    m_dialogueBox->SetPosition(500.0f, 980.0f);
    m_dialogueBox->Draw();
    App::Print(165, 710, m_displayedText.c_str(), 1.0f, 1.0f, 1.0f);
}

bool Intro::IsDone() const {
    return m_hypnotizeDone && m_currentSlide >= 3;
}