#ifndef __ENEMY_SENTIENT_DRILL_H__
#define __ENEMY_SENTIENT_DRILL_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new sentient drill at the given location
 * @param position where to spawn the drill at
 * @return NULL on failure, or a pointer to the new drill
 */
Entity* sentient_drill_new(Vector2D position);


#endif