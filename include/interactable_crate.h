#ifndef __INTERACTABLE_CRATE_H__
#define __INTERACTABLE_CRATE_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new crate at the given location
 * @param position where to spawn the crate at
 * @return NULL on failure, or a pointer to the new crate
 */
Entity* crate_new(Vector2D position);


#endif