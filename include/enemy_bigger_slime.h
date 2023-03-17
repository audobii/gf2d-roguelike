#ifndef __ENEMY_BIGGER_SLIME_H__
#define __ENEMY_BIGGER_SLIME_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new space bug at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* bigger_slime_new(Vector2D position);


#endif