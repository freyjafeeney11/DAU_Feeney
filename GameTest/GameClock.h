#pragma once

class GameClock {
private:
    float m_accum;
    int   m_hour;
    int   m_day;
    static constexpr float SECS_PER_HOUR = 10.0f;
public:
    GameClock();
    void Update(float deltaTime);
    void AdvanceToMorning();
    void SetHour(int h);
    bool IsDay()  const { return m_hour >= 6 && m_hour < 20; }
    int  GetHour() const { return m_hour; }
    int  GetDay()  const { return m_day; }
};