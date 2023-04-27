#include "simple_logger.h"
#include "interactable_water_hazard.h"

#include "player.h"
#include "projectile.h"

#include "entity_common.h"

#include "gf2d_draw.h"
#include "gf2d_collision.h"

static float internal_timer = 0;

void water_hazard_think(Entity* self);
void water_hazard_draw(Entity* self);
void water_hazard_update(Entity* self);

Entity* water_hazard_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image("images/water_tile.png");

    ent->draw = water_hazard_draw;
    ent->think = water_hazard_think;
    ent->update = water_hazard_update;

    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(32, 36);
    ent->speed = 0;
    ent->rotation = 0;
    ent->rotatable = false;

    //body/collision stuff
    ent->shape = gfc_shape_circle(0, 0, 28);
    ent->body.shape = &ent->shape;
    ent->body.team = 2;
    vector2d_copy(ent->body.position, position);

    gfc_line_cpy(ent->name, "water_hazard");
    return ent;
}

void water_hazard_draw(Entity* self) {
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

void water_hazard_think(Entity* self) {
    
}

void water_hazard_update(Entity* self) {
    Entity* other;
    Collision* collision;

    if (!player_get())return;

    other = player_get();

    collision = gf2d_collision_body_body(&self->body, &other->body);

    if (!collision)other->speed=3.5;
    else other->speed = 1.0;
    //slog("ello");
}

/*eol@eof*/