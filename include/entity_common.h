#ifndef __ENTITY_COMMON_H__
#define __ENTITY_COMMON_H__

#include "entity.h"

/**
 * @brief deal damage to an entity.  free it when its health drop to zero or below
 * @param self who takes the damage
 * @param damage how much damage to deal
 * @param inflictor who dealt the damage
 */
void entity_damage(Entity* self, float damage, Entity* inflictor);

/**
 * @brief clear entity from current level (remove from relevant lists)
 * @param ent the entity to clear
 */
void entity_clear_from_level(Entity* ent);

/**
 * @brief to be used after entity dies - drop loot for player to pick up
 * @param ent the entity to clear
 */
void entity_drop_loot(Entity* ent);

void entity_draw_still_image(Entity* ent);

/**
 * @brief check for collision and cause contact damage against any entities collided with
 * @param ent the entity doing contact damage
 */
void entity_do_contact_damage(Entity* ent);

/**
 * @brief spawn enemy entity by name
 * @param ent_name name of entity to spawn, as a textline
 * @param pos position of entity to be spawned in
 */
Entity* entity_spawn_by_name(TextLine ent_name, Vector2D pos);

#endif