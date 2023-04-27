#ifndef __INTERACTABLE_WATER_HAZARD_H__
#define __INTERACTABLE_WATER_HAZARD_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new water hazard at the given location
 * @param position where to spawn the button at
 * @return NULL on failure, or a pointer to the new hazard
 */
Entity* water_hazard_new(Vector2D position);


#endif