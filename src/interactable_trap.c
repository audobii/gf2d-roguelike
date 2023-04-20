#include "simple_logger.h"
#include "interactable_trap.h"

#include "player.h"
#include "projectile.h"

#include "entity_common.h"

#include "gf2d_draw.h"

static float internal_timer = 0;

void trap_think(Entity* self);
void trap_draw(Entity* self);

Entity* trap_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image("images/crystal_spikes.png");

    ent->draw = trap_draw;
    ent->think = trap_think;

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

    gfc_line_cpy(ent->name, "trap");
    return ent;
}

void trap_draw(Entity* self) {
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

void trap_think(Entity* self) {
    internal_timer += 0.1;

    if (internal_timer > 5.0) {
        entity_do_contact_damage(self);
        internal_timer = 0;
    }
}

/*eol@eof*/