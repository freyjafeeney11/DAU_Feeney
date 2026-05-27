#pragma once

enum class Archetype { NONE, CHARISMATIC, DEXTEROUS, LUCKY };

struct PlayerArchetype {
    static Archetype current;
};
