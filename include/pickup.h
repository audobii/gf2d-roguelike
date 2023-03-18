#ifndef __PICKUP_H__
#define __PICKUP_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new pickup at the given location
 * @param position where to spawn the pickup at
 * @param type type of pickup to spawn (1=hp, 2=mana, 3=money)
 * @return NULL on failure, or a pointer to the new pickup
 */
Entity* pickup_new(Vector2D position, int type);


#endif