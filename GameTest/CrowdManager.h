// CrowdManager.h
#pragma once
#include "app\app.h"
#include <math.h>

class CrowdManager {
private:
    static const int NUM_CLUMPS = 3;
    static const int MEMBERS_PER_CLUMP = 5;

    struct CrowdMember {
        CSimpleSprite* sprite;
        float offsetX;
        float offsetY;
        float personalSwayOffset;
    };

    struct CrowdClump {
        float baseX;
        float baseY;
        float swayOffset;
        CrowdMember members[MEMBERS_PER_CLUMP];
    };

    CrowdClump crowdClumps[NUM_CLUMPS];
    float swayTime;

public:
    CrowdManager();
    ~CrowdManager();

    void Update(float deltaTime);
    void Render(float camX, float camY);
};