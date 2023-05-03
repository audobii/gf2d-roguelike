#ifndef __INTERACTABLE_WELL_H__
#define __INTERACTABLE_WELL_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new well at the given location
 * @param position where to spawn the well at
 * @return NULL on failure, or a pointer to the new well
 */
Entity* well_new(Vector2D position);


#endif