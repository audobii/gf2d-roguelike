#ifndef __BOSS_ROYAL_SLIME_SMALLER_H__
#define __BOSS_ROYAL_SLIME_SMALLER_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new boss slime (phase2) at the given location
 * @param position where to spawn the slime at
 * @return NULL on failure, or a pointer to the new slime
 */
Entity* boss_slime_smaller_new(Vector2D position);


#endif