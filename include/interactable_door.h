#ifndef __INTERACTABLE_DOOR_H__
#define __INTERACTABLE_DOOR_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new door at the given location
 * @param position where to spawn the door at
 * @return NULL on failure, or a pointer to the new door
 */
Entity* door_new(Vector2D position);


#endif