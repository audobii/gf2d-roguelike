#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gf2d_collision.h"

#include "level.h"
#include "projectile.h"
#include "player.h"

void projectile_think(Entity* self);
void projectile_draw(Entity* self);
void projectile_update(Entity* self);

Entity* projectile_new(Entity* parent, Vector2D position, Vector2D dir, float speed, float damage, int size, int health) {
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;

    gfc_line_cpy(ent->name, "proj");
    gfc_line_cpy(ent->body.name, "proj");

    ent->think = projectile_think;
    ent->update = projectile_update;
    ent->draw = projectile_draw;

    ent->shape = gfc_shape_circle(0, 0, size);// shape position becomes offset from entity position, in this case zero
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
    ent->health = health;
    ent->damage = damage;

    level_add_entity(level_get_active_level(), ent);
    return ent;
}

void projectile_think(Entity* self) {
    if (!self)return;
    self->health--;
    if (self->health <= 0) {
        projectile_free(self);
    }
}

void projectile_draw(Entity* self) {
    if (!self)return;
    if (self->body.team == 1) {
        if (player_get_ability() == 3 && player_ability_is_active()) {
            gf2d_draw_circle(self->body.position, 20, GFC_COLOR_RED);
        } 
        else if (player_get_ability() == 5 && player_ability_is_active()) {
            gf2d_draw_circle(self->body.position, 10, gfc_color8(134, 252, 160, 255));
        }
        else {
            gf2d_draw_circle(self->body.position, 10, gfc_color8(0, 149, 255, 255));
        }
    }
    else {
        gf2d_draw_circle(self->body.position, 10, GFC_COLOR_LIGHTCYAN);
    }
}

void projectile_update(Entity* self) {
    //loop through active bodies of curr level. and check collision w each of them
    //skip over this projectile, other projectiles
    //if collide, do damage and disappear

    List* activeEnts;
    Entity* other;
    Collision* collision;

    if (!self)return;
    activeEnts = level_get_active_level()->activeEntities;

    if (!activeEnts)return;

    for (int i = 0; i < gfc_list_get_count(activeEnts); i++) {
        other = gfc_list_get_nth(activeEnts, i);

        if (other == self)continue;
        //if (!gfc_line_cmp(other->body.name, "proj"))continue;
        if (other->body.team == self->body.team)continue;

        collision = gf2d_collision_body_body(&self->body, &other->body);

        if (!collision)continue;
        //slog("owie");

        //print dmg before it goes into takeDamage function - it works and is correct
        //char str[20];
        //sprintf(str, "%f", self->damage);
        //slog(str);

        //do damage
        if (other->takeDamage)other->takeDamage(other, self->damage, self);
        //slog(other->name);

        projectile_free(self);
        return;
    }

}

void projectile_free(Entity* ent) {
    gfc_list_delete_data(level_get_active_level()->activeBodies, &ent->body);
    gfc_list_delete_data(level_get_active_level()->activeEntities, ent);
    entity_free(ent);
}