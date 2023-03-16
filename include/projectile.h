#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "gfc_vector.h"

#include "entity.h"

/**
 * @brief spawn a new projectile at the given location
 * @param parent the entity that fired the projectile
 * @param position where to spawn the projectile at
 * @param dir the direction to move in
 * @param speed how fast we move
 * @param damage how much damage we will
 * @return NULL on failure, or a pointer to the new projectile
 */
Entity* projectile_new(Entity* parent, Vector2D position, Vector2D dir, float speed, float damage);

/**
 * @brief free a previously allocated projectile entity after removing it from related lists
 * @param ent the entity to free
 */
void projectile_free(Entity* ent);

#endif