#ifndef __SLIME_H__
#define __SLIME_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new slime at the given location
 * @param position where to spawn the slime at
 * @return NULL on failure, or a pointer to the new slime
 */
Entity* slime_new(Vector2D position);


#endif