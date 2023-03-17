#include "simple_logger.h"
#include "entity_common.h"

#include "level.h"
#include "player.h"
#include "slime.h"

void entity_damage(Entity* self, float damage, Entity* inflictor)
{
    if (!self)return;
    if (self->health < 0)return;// lets not beat a dead horse
    slog("hit");

    //printing damage variable... its 0.00000
    //char str[20];
    //sprintf(str, "%f", damage);
    //slog(str);

    //TODO: fix this.
    //damage being passed to this function is not working for some reason... TEMPORARY HARD CODING VALUE
    if (player_ability_is_active() && player_get_ability() == 4 && self->body.team != 1) { //for ability 4 - just insta kill them
        self->health = 0;
    }
    else if (player_ability_is_active() && player_get_ability() == 5 && self->body.team != 1) { //for ability 5 - poison and do a little damage
        self->health -= 1;
        self->isPoisoned = true;
    }
    else {
        self->health -= 10; //hardcoded, supposed to be damage 
    }

    if (self->health <= 0)
    {
        slog("died...");
        if (!gfc_line_cmp(self->name, "big_slime")) { //if big slime, split into two smaller ones
            Vector2D offset_pos;
            offset_pos = vector2d(self->position.x + 40, self->position.y + 40);
            level_add_entity(level_get_active_level(), slime_new(self->position));
            level_add_entity(level_get_active_level(), slime_new(offset_pos));
        }
        entity_clear_from_level(self);
        entity_free(self);
    }
}

void entity_clear_from_level(Entity* ent) {
    gfc_list_delete_data(level_get_active_level()->activeBodies, &ent->body);
    gfc_list_delete_data(level_get_active_level()->activeEntities, ent);
}

void entity_drop_loot(Entity* ent) {
    //TODO
    //to be added: currency, health/mana recovery items, boosts/powerups?
}