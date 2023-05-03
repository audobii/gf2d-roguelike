#include "simple_logger.h"
#include "interactable_button.h"

#include "player.h"
#include "projectile.h"

#include "entity_common.h"

#include "gf2d_draw.h"

#include "level.h"
#include "gf2d_collision.h"

static float internal_timer = 0;
static Bool collided = false; //check for collision
static Bool triggered = false; //only enable button press once
static Entity* button_trigger = NULL;

void button_think(Entity* self);
void button_draw(Entity* self);
void button_update(Entity* self);

Entity* button_new(Vector2D position, Entity* trigger)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image("images/button.png");

    ent->draw = button_draw;
    ent->think = button_think;
    ent->update = button_update;

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

    gfc_line_cpy(ent->name, "button");

    button_trigger = trigger;

    return ent;
}

void button_draw(Entity* self) {
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

void button_think(Entity* self) {

    //might not need internal timer, just trigger it once
    /*
    internal_timer += 0.1;

    if (internal_timer > 5.0) {
        //copy projectile update? or make separate function to check for collisions and call it here
        internal_timer = 0;
    }
    */

    if (collided && !triggered) {
        //slog("hiiii");

        //trigger one-time something here

        entity_free(button_trigger);

        collided = false;
        triggered = true;
    }
}

void button_update(Entity* self) {
    Entity* other;
    Collision* collision;

    if (!player_get())return;

    other = player_get();

    collision = gf2d_collision_body_body(&self->body, &other->body);

    if (!collision)return;

    collided = true;
}

/*eol@eof*/