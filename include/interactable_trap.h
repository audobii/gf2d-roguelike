#ifndef __INTERACTABLE_TRAP_H__
#define __INTERACTABLE_TRAP_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new crystal trap at the given location
 * @param position where to spawn the trap at
 * @return NULL on failure, or a pointer to the new trap
 */
Entity* trap_new(Vector2D position);


#endif