#ifndef __ENEMY_ROCK_BLOCK_H__
#define __ENEMY_ROCK_BLOCK_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new angry rock block at the given location
 * @param position where to spawn the block at
 * @return NULL on failure, or a pointer to the new block
 */
Entity* rock_block_new(Vector2D position);


#endif