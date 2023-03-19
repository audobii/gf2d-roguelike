#ifndef __SHOP_ITEM_H__
#define __SHOP_ITEM_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new shop item at the given location
 * @param position where to spawn the item at
 * @param type type of item to spawn (1=triple; 2=heal; 3=self destruct; 4=rage; 5=poison; 6=heart; 7=mana;)
 * @return NULL on failure, or a pointer to the new item
 */
Entity* shop_item_new(Vector2D position, int type);

int shop_item_get_price(Entity* self);

#endif