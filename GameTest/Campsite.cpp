#include "stdafx.h"
#include "Campsite.h"
#include <math.h>

Rooftop::Rooftop() {
    m_background = App::CreateSprite(".\\TestData\\camp.png", 1, 1);
    m_background->SetPosition(512.0f, 400.0f);
    m_background->SetScale(0.6f);
}

Rooftop::~Rooftop() {
    delete m_background;
}

void Rooftop::Update(float deltaTime) {
}

void Rooftop::Render() {
    m_background->SetPosition(512.0f, 400.0f);
    m_background->Draw();
}

bool Rooftop::IsPlayerNearHatch(float px) const {
    return fabsf(px - HATCH_X) < HATCH_RADIUS;
}