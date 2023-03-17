#ifndef __ENEMY_GLOWFLY_H__
#define __ENEMY_GLOWFLY_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new glowfly at the given location
 * @param position where to spawn the glowfly at
 * @return NULL on failure, or a pointer to the new glowfly
 */
Entity* glowfly_new(Vector2D position);


#endif