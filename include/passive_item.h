#ifndef __PASSIVE_ITEM_H__
#define __PASSIVE_ITEM_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new passive item at the given location
 * @param position where to spawn the item at
 * @param type type of item to spawn 
 * @return NULL on failure, or a pointer to the new item
 */
Entity* passive_item_new(Vector2D position, int type);

#endif