//collision type für movement um einzustellen, welche objekte bei bewegung eines objekts dieses blockieren
#pragma once
enum COLLISION_TYPE{
    NONE,
    BLOCKED_BY_UPPER_DISPLAY_SIDE,
    BLOCKED_BY_LOWER_DISPLAY_SIDE,
    BLOCKED_BY_LEFT_DISPLAY_SIDE,
    BLOCKED_BY_RIGTH_DISPLAY_SIDE,
};

enum ColliderType{
    BASE_COLLIDER,
    BOXCOLLIDER,
    CIRCLECOLLIDER,
};