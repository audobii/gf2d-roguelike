#include "simple_logger.h"
#include "interactable_well.h"

#include "player.h"
#include "projectile.h"

#include "entity_common.h"

#include "gf2d_draw.h"

#include "gf2d_collision.h"

static float internal_timer = 0;

void well_think(Entity* self);
void well_draw(Entity* self);
void well_update(Entity* self);

Entity* well_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image("images/well.png");

    ent->draw = well_draw;
    ent->think = well_think;
    ent->update = well_update;

    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(64, 68);
    ent->speed = 0;
    ent->rotation = 0;
    ent->rotatable = false;

    //body/collision stuff
    ent->shape = gfc_shape_rect(-64,-68, 128, 128);
    ent->body.shape = &ent->shape;
    ent->body.team = 2;
    vector2d_copy(ent->body.position, position);

    gfc_line_cpy(ent->name, "well");
    return ent;
}

void well_draw(Entity* self) {
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

void well_think(Entity* self) {
    internal_timer += 0.1;

    if (internal_timer > 10.0) {
        Entity* other;
        Collision* collision;

        if (!player_get())return;

        other = player_get();

        collision = gf2d_collision_body_body(&self->body, &other->body);

        if (collision) {
            if(player_get_mana() < 350)player_set_mana(player_get_mana() + 1);
        }
    }
}

void well_update(Entity* self) {

}

/*eol@eof*/