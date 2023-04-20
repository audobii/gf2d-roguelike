#include "simple_logger.h"
#include "gf2d_draw.h"
#include "entity.h"

#include "level.h"

typedef struct
{
    Uint32  entity_max;
    Entity* entity_list;
    SJson* entity_def;
}EntityManager;

//similar in quake with the list of entities
static EntityManager entity_manager = { 0 };

void entity_manager_close()
{
    entity_free_all();
    if (entity_manager.entity_list)free(entity_manager.entity_list);
    slog("entity system closed");
}

void entity_manager_init(Uint32 max)
{
    if (max <= 0)
    {
        slog("cannot intialize entity system: zero entities specified!");
        return;
    }
    entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), max);
    if (!entity_manager.entity_list)
    {
        slog("failed to initialize entity system!");
        return;
    }
    entity_manager.entity_max = max;
    entity_manager.entity_def = sj_load("config/entities.def");
    atexit(entity_manager_close);
    slog("entity system initialized");
}

void entity_free_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_max; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_free(&entity_manager.entity_list[i]);
    }
}

Entity* entity_new()
{
    int i;
    for (i = 0; i < entity_manager.entity_max; i++)
    {
        if (entity_manager.entity_list[i]._inuse)continue;
        entity_manager.entity_list[i]._inuse = 1;
        return &entity_manager.entity_list[i];
    }
    return NULL;
}

void entity_free(Entity* ent)
{
    if (!ent)
    {
        return;
    }
    if (ent->sprite)gf2d_sprite_free(ent->sprite);
    memset(ent, 0, sizeof(Entity));
}

void entity_draw(Entity* ent, Bool debug)
{
    if (!ent)return;

    if (ent->draw) { //if a defined draw function exists for the ent (like player)
        ent->draw(ent);
    }
    else if (ent->sprite)
    {
        gf2d_sprite_draw(
            ent->sprite,
            ent->position,
            NULL,
            &ent->drawOffset,
            &ent->rotation,
            NULL,
            NULL,
            (Uint32)ent->frame);

        //uncomment to see hitboxes/shapes
        // TODO: in the future make a "debug" mode to draw these
        if (debug) {
            gf2d_draw_pixel(ent->position, GFC_COLOR_YELLOW);
            gf2d_draw_circle(ent->position, 10, GFC_COLOR_YELLOW);
        }
    }
    //gf2d_draw_pixel(ent->position, GFC_COLOR_YELLOW);
    //gf2d_draw_circle(ent->position, 10, GFC_COLOR_YELLOW);
    if(debug)body_draw(&ent->body, vector2d(0, 0));
}

void entity_draw_all(Bool debug)
{
    int i;
    for (i = 0; i < entity_manager.entity_max; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_draw(&entity_manager.entity_list[i], debug);
    }
}

void entity_update(Entity* ent)
{
    //somewhere here? check if body doesnt exist. if it doesnt delete the whole entity

    if (!ent)return;

    //need to delete data from list before freeing ENTITIES, so...
    if (!gfc_line_cmp(ent->body.name, "FREEME")) {
        //slog("freeing?");
        gfc_list_delete_data(level_get_active_level()->activeBodies, &ent->body);
        gfc_list_delete_data(level_get_active_level()->activeEntities, ent);
        entity_free(ent);
    }
    else {
        if (ent->update)
        {
            ent->update(ent);// if the update function returns 1, do not do generic update
        }
        ent->frame += 0.1;
        if (ent->frame >= 16)ent->frame = 0;
        vector2d_add(ent->position, ent->position, ent->velocity);
        if (vector2d_magnitude_compare(ent->velocity, 0) != 0)
        {
            //means the vector is non zero
            //slog("GFC_PI: %f", GFC_PI);
            if (ent->rotatable) {
                ent->rotation = (vector2d_angle(ent->velocity) + 180);
            }
            //        angle_clamp_radians(&ent->rotation);
        }

        //update body stuff as well
        vector2d_add(ent->body.position, ent->body.position, ent->body.velocity);
        //maybe a hacky way to make sure sprite is also doing the collision stuff
        ent->position = ent->body.position;
    }
}

void entity_update_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_max; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_update(&entity_manager.entity_list[i]);
    }
}

void entity_think(Entity* ent)
{
    if (!ent)return;
    if (ent->think)ent->think(ent);
}

void entity_think_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_max; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)continue;
        entity_think(&entity_manager.entity_list[i]);
    }
}

SJson* entity_get_def_by_name(const char* name)
{
    //    int i,c;
    if (!name)return NULL;

    return NULL;
}

Shape entity_get_shape_after_move(Entity* ent)
{
    Shape shape = { 0 };
    if (!ent)return shape;
    gfc_shape_copy(&shape, ent->shape);
    gfc_shape_move(&shape, ent->position);
    gfc_shape_move(&shape, ent->velocity);
    return shape;
}

Shape entity_get_shape(Entity* ent)
{
    Shape shape = { 0 };
    if (!ent)return shape;
    gfc_shape_copy(&shape, ent->shape);
    gfc_shape_move(&shape, ent->position);
    return shape;
}


/*eol@eof*/