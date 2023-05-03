#include "simple_logger.h"
#include "interactable_crate.h"

#include "player.h"
#include "projectile.h"

#include "entity_common.h"

#include "gf2d_draw.h"

#include "gf2d_collision.h"
#include "level.h"

static float internal_timer = 0;

void crate_think(Entity* self);
void crate_draw(Entity* self);
void crate_update(Entity* self);

Entity* crate_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_image("images/crate.png");

    ent->draw = crate_draw;
    ent->think = crate_think;
    ent->update = crate_update;
    ent->takeDamage = entity_damage;

    vector2d_copy(ent->position, position);
    ent->drawOffset = vector2d(32, 36);
    ent->speed = 0;
    ent->rotation = 0;
    ent->rotatable = false;

    //body/collision stuff
    ent->shape = gfc_shape_rect(-26, -30, 54, 54);
    ent->body.shape = &ent->shape;
    ent->body.team = 2;
    vector2d_copy(ent->body.position, position);

    ent->health = 30;

    gfc_line_cpy(ent->name, "crate");
    return ent;
}

void crate_draw(Entity* self) {
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

void crate_think(Entity* self) {
    Entity* other;
    Collision* collision;
    Body* b;

    if (!player_get())return;

    other = player_get();

    collision = gf2d_collision_body_body(&self->body, &other->body);

    if (!collision)return;

    b = &other->body;

    vector2d_sub(b->position, b->position, b->velocity);

    if (self->health <= 0) {
        for (int i = -1; i < 2; i++) {
            int rand_num = (rand() % 3) + 1;

            pickup_new(vector2d(self->position.x + (5*i), self->position.y + (5 * i)), rand_num);
        }

        entity_free(self);
    }
}

void crate_update(Entity* self) {
    List* activeEnts;
    Entity* other;
    Collision* collision;
    Body* b;

    if (!self)return;
    activeEnts = level_get_active_level()->activeEntities;

    if (!activeEnts)return;

    for (int i = 0; i < gfc_list_get_count(activeEnts); i++) {
        other = gfc_list_get_nth(activeEnts, i);

        if (other == self)continue;
        if (other->body.team == self->body.team)continue;

        collision = gf2d_collision_body_body(&self->body, &other->body);

        if (!collision)continue;

        b = &other->body;

        if (!gfc_line_cmp(b->name, "proj")) {
            self->health -= 10;
            gfc_line_cpy(b->name, "FREEME");
        }

        if (self->health <= 0) {
            /*
            for (int j = -1; j < 2; j++) {
                int rand_num = (rand() % 3) + j;

                pickup_new(vector2d(self->position.x + (15 * j), self->position.y + (15 * j)), rand_num);
            }
            */

            pickup_new(vector2d(self->position.x - 20, self->position.y), (rand() % 3) + 1);
            pickup_new(vector2d(self->position.x + 20, self->position.y), (rand() % 3) + 1);
            pickup_new(vector2d(self->position.x, self->position.y - 15), (rand() % 3) + 1);

            entity_free(self);
        }

        return;
    }
}

/*eol@eof*/