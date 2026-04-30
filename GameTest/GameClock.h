#pragma once

class GameClock {
private:
    CSimpleSprite* m_background;
    float m_accum;
    int   m_hour;
    int   m_day;
    static constexpr float SECS_PER_HOUR = 10.0f;
public:
    GameClock();
    ~GameClock();
    void Update(float deltaTime);
    void AdvanceToMorning();
    void SetHour(int h);
    void Reset();
    bool IsDay()  const { return m_hour >= 9 && m_hour < 17; }
    int  GetHour() const { return m_hour; }
    int  GetDay()  const { return m_day; }
    void Render();
};