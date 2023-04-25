#ifndef __INTERACTABLE_BUTTON_H__
#define __INTERACTABLE_BUTTON_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new button at the given location
 * @param position where to spawn the button at
 * @param trigger what to trigger with button
 * @return NULL on failure, or a pointer to the new button
 */
Entity* button_new(Vector2D position, Entity* trigger);


#endif