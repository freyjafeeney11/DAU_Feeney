#include "stdafx.h"
#include "GameClock.h"

GameClock::GameClock() {
    m_accum = 0.0f;
    m_hour = 20;
    m_day = 1;
}

void GameClock::Update(float deltaTime) {
    m_accum += deltaTime / 1000.0f;
    if (m_accum >= SECS_PER_HOUR) {
        m_accum -= SECS_PER_HOUR;
        m_hour++;
        if (m_hour >= 24) {
            m_hour = 0;
            m_day++;
        }
    }
}

void GameClock::AdvanceToMorning() {
    m_hour = 6;
    m_accum = 0.0f;
    m_day++;
}

void GameClock::SetHour(int h) {
    m_hour = h;
    m_accum = 0.0f;
}