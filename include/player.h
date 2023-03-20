#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new player at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* player_new(Vector2D position);

/**
 * @brief get a pointer to the active player, if available
 * @return the player or NULL if not set
 */
Entity* player_get();

/**
 * @brief get the player's position (or a zero vector if none is set);
 * @return the player's position
 */
Vector2D player_get_position();

/**
 * @brief set the player's position;
 * @param newPos the new position to set player to
 */
void player_set_position(Vector2D newPos);

/**
 * @brief get player's current mana value
 * @return the player's mana
 */
Uint32 player_get_mana();

/**
 * @brief set player's mana value
 * @param newMana the new value to set player's mana to
 */
void player_set_mana(Uint32 newMana);

/**
 * @brief get player's current money amount
 * @return the player's money
 */
int player_get_money();

/**
 * @brief set player's money amount
 * @param newMoney the new value to set player's money to
 */
void player_set_money(int newMoney);

/**
 * @brief draw player's HUD
 */
void player_draw_hud(Entity* self);

void player_activate_ability();

void player_deactivate_ability();

int player_get_ability();

void player_set_ability(Entity* self, int ability);
/**
 * @return true if player ability is active; false if otherwise
 */
Bool player_ability_is_active();

/**
 * @brief trigger game over if player dies
 */
void player_game_over(Entity* self);

#endif