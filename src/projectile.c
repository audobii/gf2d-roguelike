#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gf2d_collision.h"

#include "level.h"
#include "projectile.h"

void projectile_think(Entity* self);
void projectile_draw(Entity* self);
void projectile_update(Entity* self);

Entity* projectile_new(Entity* parent, Vector2D position, Vector2D dir, float speed, float damage) {
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    gfc_line_cpy(ent->name, "proj");
    gfc_line_cpy(ent->body.name, "proj");

    ent->think = projectile_think;
    //ent->update = projectile_update;
    ent->draw = projectile_draw;

    ent->shape = gfc_shape_circle(0, 0, 5);// shape position becomes offset from entity position, in this case zero
    ent->body.shape = &ent->shape;

    if (parent)
    {
        ent->body.team = parent->body.team;
    }

    vector2d_copy(ent->body.position, position);
    vector2d_normalize(&dir);
    vector2d_scale(ent->body.velocity, dir, speed);

    ent->rotation += GFC_HALF_PI;
    ent->speed = speed;
    ent->health = 100;
    ent->damage = damage;

    level_add_entity(level_get_active_level(), ent);
    return ent;
}

void projectile_think(Entity* self) {
    if (!self)return;
    self->health--;
    if (self->health <= 0) {
        gfc_list_delete_data(level_get_active_level()->activeBodies, &self->body);
        gfc_list_delete_data(level_get_active_level()->activeEntities, self);
        entity_free(self);
    }
}

void projectile_draw(Entity* self) {
    if (!self)return;
    gf2d_draw_circle(self->body.position, 10, GFC_COLOR_RED);
}

void projectile_update(Entity* self) {

}