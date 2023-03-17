#ifndef __ENEMY_BIGGER_SLIME_H__
#define __ENEMY_BIGGER_SLIME_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new bigger slime bug at the given location
 * @param position where to spawn the slime at
 * @return NULL on failure, or a pointer to the new slime
 */
Entity* bigger_slime_new(Vector2D position);


#endif