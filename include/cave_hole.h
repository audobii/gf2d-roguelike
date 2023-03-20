#ifndef __CAVE_HOLE_H__
#define __CAVE_HOLE_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new cave hole at the given location
 * @param position where to spawn the cave hole at
 * @return NULL on failure, or a pointer to the new cave hole
 */
Entity* cave_hole_new(Vector2D position);


#endif