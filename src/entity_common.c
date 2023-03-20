#include "simple_logger.h"
#include "entity_common.h"

#include "level.h"
#include "player.h"
#include "gf2d_collision.h"
#include "pickup.h"

#include "slime.h"
#include "enemy_bigger_slime.h"
#include "enemy_sentient_drill.h"
#include "enemy_crystal_guy.h"
#include "enemy_glowfly.h"
#include "enemy_rock_block.h"

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

    //TODO: maybe make a die function for each entity and call it here; call a generic one if they dont have one
    if (self->health <= 0)
    {
        slog("died...");
        if (!gfc_line_cmp(self->name, "big_slime")) { //if big slime, split into two smaller ones
            Vector2D offset_pos;
            offset_pos = vector2d(self->position.x + 40, self->position.y + 40);
            level_add_entity(level_get_active_level(), slime_new(self->position));
            level_add_entity(level_get_active_level(), slime_new(offset_pos));
        }
        else if (!gfc_line_cmp(self->name, "player")) { //if player dies handle it differently?
            player_game_over(self);
        }
        entity_clear_from_level(self);
        if (gfc_line_cmp(self->name, "player"))entity_drop_loot(self);
        entity_free(self);
    }
}

void entity_clear_from_level(Entity* ent) {
    if (!ent)return;
    gfc_list_delete_data(level_get_active_level()->activeBodies, &ent->body);
    gfc_list_delete_data(level_get_active_level()->activeEntities, ent);
}

void entity_drop_loot(Entity* ent) {
    //TODO
    //to be added: currency, health/mana recovery items, boosts/powerups?
    int rand_num = (rand() % 3) + 1;

    pickup_new(ent->position, rand_num);
}

void entity_draw_still_image(Entity* ent) {
    //TODO
    //consolidate the code for all the entities that just draw a still image (player, some enemies)
}

void entity_do_contact_damage(Entity* ent) {
    //copied over from projectile update
    //for enemies mostly
    List* activeEnts;
    Entity* other;
    Collision* collision;

    activeEnts = level_get_active_level()->activeEntities;

    if (!activeEnts)return;

    for (int i = 0; i < gfc_list_get_count(activeEnts); i++) {
        other = gfc_list_get_nth(activeEnts, i);

        if (other == ent)continue;
        if (!gfc_line_cmp(other->body.name, "proj"))continue;
        if (other->body.team == ent->body.team)continue;

        collision = gf2d_collision_body_body(&ent->body, &other->body);

        if (!collision)continue;
        //slog("owie");

        //do damage
        if (other->takeDamage)other->takeDamage(other, 30, ent);
        //slog(other->name);
    }
}

Entity* entity_spawn_by_name(TextLine ent_name, Vector2D pos) {
    Entity* new_ent = NULL;

    if (!gfc_line_cmp(ent_name, "slime")) {
        new_ent = slime_new(pos);
    }
    else if (!gfc_line_cmp(ent_name, "bigger_slime")) {
        new_ent = bigger_slime_new(pos);
    }
    else if (!gfc_line_cmp(ent_name, "crystal_guy")) {
        new_ent = crystal_guy_new(pos);
    }
    else if (!gfc_line_cmp(ent_name, "glowfly")) {
        new_ent = glowfly_new(pos);
    }
    else if (!gfc_line_cmp(ent_name, "rock_block")) {
        new_ent = rock_block_new(pos);
    }
    else if (!gfc_line_cmp(ent_name, "sentient_drill")) {
        new_ent = sentient_drill_new(pos);
    }

    //if (!new_ent)return;
    //level_add_entity(level_get_active_level(), new_ent); //use if void method 
    return new_ent;
}