#include "simple_logger.h"
#include "entity_common.h"

#include "level.h"
#include "player.h"

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
    else {
        self->health -= 10; //hardcoded, supposed to be damage 
    }

    if (self->health <= 0)
    {
        slog("died...");
        entity_clear_from_level(self);
        entity_free(self);
    }
}

void entity_clear_from_level(Entity* ent) {
    gfc_list_delete_data(level_get_active_level()->activeBodies, &ent->body);
    gfc_list_delete_data(level_get_active_level()->activeEntities, ent);
}