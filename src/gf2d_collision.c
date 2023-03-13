#include <stdlib.h>
#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gf2d_collision.h"

Collision* collision_new() {
    Collision* collision = NULL;
    collision = (Collision*)malloc(sizeof(Collision));
    if (!collision)
    {
        slog("failed to allocate data for a collision object");
        return NULL;
    }
    memset(collision, 0, sizeof(Collision));
    return collision;
}


