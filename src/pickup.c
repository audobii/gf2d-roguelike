#include "simple_logger.h"
#include "pickup.h"

#include "player.h"

#include "entity_common.h"
#include "gf2d_collision.h"

static float internal_timer = 0;

void pickup_think(Entity* self);
void pickup_draw(Entity* self);

//1 = hp; 2 = mana; 3 = money
Entity* pickup_new(Vector2D position, int type)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    switch (type) {
    case 1:
        ent->sprite = gf2d_sprite_load_image("images/heart.png");
        gfc_line_cpy(ent->name, "heart");
        break;
    case 2:
        ent->sprite = gf2d_sprite_load_image("images/mana_drop.png");
        gfc_line_cpy(ent->name, "mana");
        break;
    case 3:
        ent->sprite = gf2d_sprite_load_image("images/coin.png");
        gfc_line_cpy(ent->name, "money");
        break;
    default:
        return;
        break;
    }

    ent->think = pickup_think;
    ent->draw = pickup_draw;

    vector2d_copy(ent->position, position);

    ent->drawOffset = vector2d(8, 8);
    ent->speed = 0;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 8);
    ent->body.shape = &ent->shape;
    ent->body.team = 0;
    vector2d_copy(ent->body.position, position);

    //gfc_line_cpy(ent->name, "pickup");
    return ent;
}

void pickup_draw(Entity* self) {
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

    gf2d_draw_pixel(self->position, gfc_color8(255, 255, 255, 160));
    gf2d_draw_circle(self->position, 10, gfc_color8(255, 255, 255, 160));
}

void pickup_think(Entity* self)
{
    //just keep checking if it makes contact with player
    List* activeEnts;
    Entity* player;
    Collision* collision;
    Uint32 p_mana;

    player = player_get();

    if (!player)return;
    collision = gf2d_collision_body_body(&self->body, &player->body);
    p_mana = player_get_mana();

    if (!collision)return;
    if (!gfc_line_cmp(self->name, "heart")) {
        if (player->health < 330) {
            player->health += 20;
        }
        else {
            player->health = 350;
        }
    }else if (!gfc_line_cmp(self->name, "mana")) {
        if (p_mana < 330) {
            player_set_mana(player_get_mana() + 20);
        }
        else {
            player_set_mana(350);
        }
    }else if (!gfc_line_cmp(self->name, "money")) {
        player_set_money(player_get_money() + 1);
    }
    //entity_clear_from_level(self);
    entity_free(self);
}

/*eol@eof*/