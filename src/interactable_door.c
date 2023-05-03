#include "simple_logger.h"
#include "interactable_door.h"

#include "player.h"
#include "projectile.h"

#include "entity_common.h"

#include "gf2d_draw.h"

#include "gf2d_collision.h"

static float internal_timer = 0;

void door_think(Entity* self);
void door_draw(Entity* self);
void door_update(Entity* self);

Entity* door_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image("images/door.png");

    ent->draw = door_draw;
    ent->think = door_think;
    ent->update = door_update;

    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(32, 36);
    ent->speed = 0;
    ent->rotation = 0;
    ent->rotatable = false;

    //body/collision stuff
    ent->shape = gfc_shape_rect(-32, -36, 64, 64);
    ent->body.shape = &ent->shape;
    ent->body.team = 2;
    vector2d_copy(ent->body.position, position);

    gfc_line_cpy(ent->name, "door");
    return ent;
}

void door_draw(Entity* self) {
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

void door_think(Entity* self) {
    Entity* other;
    Collision* collision;
    Body* b;

    if (!player_get())return;

    other = player_get();

    collision = gf2d_collision_body_body(&self->body, &other->body);

    if (!collision)return;

    b = &other->body;

    vector2d_sub(b->position, b->position, b->velocity);
}

void door_update(Entity* self) {
    
}

/*eol@eof*/