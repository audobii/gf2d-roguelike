#include "simple_logger.h"
#include "passive_item.h"

#include "player.h"

#include "entity_common.h"
#include "gf2d_collision.h"

static int passive_max = 5; //how many items in the pool

void passive_item_think(Entity* self);
void passive_item_draw(Entity* self);

Entity* passive_item_new_rand(Vector2D position)
{
    Entity* ent;

    ent = entity_new();
    if (!ent)return NULL;

    //randomize item
    int type = (rand() % passive_max) + 1;

    switch (type) {
    case 1:
        ent->sprite = gf2d_sprite_load_image("images/mana_regen_item_small.png");
        gfc_line_cpy(ent->name, "pass_mana_regen");
        break;
    default:
        return;
        break;
    }

    ent->think = passive_item_think;
    ent->draw = passive_item_draw;

    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(28, 32);
    ent->speed = 0;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 22);
    ent->body.shape = &ent->shape;
    ent->body.team = 0;
    vector2d_copy(ent->body.position, position);

    //gfc_line_cpy(ent->name, "shop_item");
    return ent;
}

Entity* passive_item_new(Vector2D position, int type)
{
    Entity* ent;

    ent = entity_new();
    if (!ent)return NULL;

    switch (type) {
    case 1:
        ent->sprite = gf2d_sprite_load_image("images/mana_regen_item_small.png");
        gfc_line_cpy(ent->name, "pass_mana_regen");
        break;
    default:
        return;
        break;
    }

    ent->think = passive_item_think;
    ent->draw = passive_item_draw;

    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(28, 32);
    ent->speed = 0;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 22);
    ent->body.shape = &ent->shape;
    ent->body.team = 0;
    vector2d_copy(ent->body.position, position);

    //gfc_line_cpy(ent->name, "shop_item");
    return ent;
}

void passive_item_draw(Entity* self) {
    if (!self)return;
    gf2d_sprite_draw(
        self->sprite,
        self->position,
        NULL,
        &self->drawOffset,
        &self->rotation,
        NULL,
        NULL,
        0);

    //gf2d_draw_pixel(self->position, gfc_color8(255, 255, 255, 160));
    //gf2d_draw_circle(self->position, 10, gfc_color8(255, 255, 255, 160));
}

void passive_item_think(Entity* self)
{
    //just keep checking if it makes contact with player
    List* activeEnts;
    Entity* player;
    Collision* collision;
    int p_current_money;
    int price;
    Uint32 p_mana;

    player = player_get();

    if (!player)return;
    collision = gf2d_collision_body_body(&self->body, &player->body);

    if (!collision)return;

    if (!gfc_line_cmp(self->name, "pass_mana_regen")) {
        //set mana regen in player to true
        player_set_manaRegen(true);
    }
    entity_free(self);
}


/*eol@eof*/