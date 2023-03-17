#ifndef __ENEMY_CRYSTAL_GUY_H__
#define __ENEMY_CRYSTAL_GUY_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new crystal guy at the given location
 * @param position where to spawn the crystal at
 * @return NULL on failure, or a pointer to the new crystal
 */
Entity* crystal_guy_new(Vector2D position);


#endif