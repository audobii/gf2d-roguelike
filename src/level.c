#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_config.h"
#include "gfc_list.h"

#include "gf2d_graphics.h"

#include "level.h"
#include "entity.h"
#include "body.h"

#include "entity_common.h"
#include "shop_item.h"

void level_build(Level* level);
void level_spawn_enemies(Level* level);

static Level* activeLevel = NULL;

Level* level_get_active_level() {
	return activeLevel;
}

void level_set_active_level(Level *level) {
	activeLevel = level;
}

Level* level_load(const char* filename) {
    int tile;
    int i, c;
    int j, d;
    int a, b;
    int x = 0, y = 0;
    int t = 0;
    int tileFPL;
    const char* str;
    SJson* json, * lj, * list, * list2, * row, * row2, * item;
    Level* level;
    List* coords;

    if (!filename)return NULL;

    //load in data from json file
    json = sj_load(filename);
    if (!json)return NULL;
    level = level_new();

    if (!level)
    {
        slog("failed to allocate new level");
        sj_free(json);
        return NULL;
    }

    lj = sj_object_get_value(json, "level");
    if (!lj)
    {
        slog("file %s missing level object", filename);
        sj_free(json);
        level_free(level);
        return NULL;
    }

    str = sj_object_get_value_as_string(lj, "name");
    if (str)gfc_line_cpy(level->name, str);

    sj_value_as_vector2d(sj_object_get_value(lj, "tileSize"), &level->tileSize);
    sj_object_get_value_as_int(lj, "tileFPL", &tileFPL);
    //load in first tileSet
    str = sj_object_get_value_as_string(lj, "tileSet");
    if (str)
    {
        level->tileSet = gf2d_sprite_load_all(str, (Sint32)level->tileSize.x, (Sint32)level->tileSize.y, tileFPL, 1);
    }

    //load in alt tileSet; dont load in if there isnt one
    str = sj_object_get_value_as_string(lj, "tileSetAlt");
    if (str)
    {
        level->tileSetAlt = gf2d_sprite_load_all(str, (Sint32)level->tileSize.x, (Sint32)level->tileSize.y, tileFPL, 1);
    }

    //load in list of enemies to spawn eventually
    //start list
    //iterate through json list
    //entity_spawn_by_name() for each thing in level_spawn_enemies
    list = sj_object_get_value(lj, "enemy_list");
    list2 = sj_object_get_value(lj, "enemy_coords");
    if (list && list2) {
        c = sj_array_get_count(list); //total enemies in level
        
        for (int i = 0; i < c; i++) {
            coords = gfc_list_new();
            row = sj_array_get_nth(list, i); //the row/enemy
            row2 = sj_array_get_nth(list2, i); //the coords

            const char* enemy_name = sj_get_string_value(row);
            //slog(enemy_name);
            a = sj_array_get_nth(row2, 0); //enemy pos x 
            b = sj_array_get_nth(row2, 1); //enemy pos y

            sj_get_integer_value(a, &x);
            sj_get_integer_value(b, &y);

            char str[20];
            sprintf(str, "%i", x);
            slog(str);

            gfc_list_append(level->enemiesToSpawn, entity_spawn_by_name(enemy_name, vector2d(x,y)));
        }
    }

    //spawn in entities for level similar to spawning enemies
    list = sj_object_get_value(lj, "entity_list");
    list2 = sj_object_get_value(lj, "entity_coords");
    if (list && list2) {
        c = sj_array_get_count(list); //total entities in level

        for (int i = 0; i < c; i++) {
            coords = gfc_list_new();
            row = sj_array_get_nth(list, i); //the entity
            row2 = sj_array_get_nth(list2, i); //the coords

            //slog(enemy_name);
            a = sj_array_get_nth(row2, 0); //enemy pos x 
            b = sj_array_get_nth(row2, 1); //enemy pos y

            sj_get_integer_value(row, &t);
            sj_get_integer_value(a, &x);
            sj_get_integer_value(b, &y);

            shop_item_new(vector2d(x, y), t);
        }
    }

    list = sj_object_get_value(lj, "tileMap");
    c = sj_array_get_count(list);
    row = sj_array_get_nth(list, 0);
    d = sj_array_get_count(row);

    if ((c * d) == 0)
    {
        slog("corrupt row or column count for %s level", filename);
        level_free(level);
        sj_free(json);
        return NULL;
    }

    level->mapSize.x = d;
    level->mapSize.y = c;
    level->tileMap = gfc_allocate_array(sizeof(int), c * d);
    if (!level->tileMap)
    {
        slog("failed to allocate tileMap for level %s", filename);
        level_free(level);
        sj_free(json);
        return NULL;
    }
    for (i = 0; i < c; i++)// i is row
    {
        row = sj_array_get_nth(list, i);
        if (!row)continue;
        d = sj_array_get_count(row);
        for (j = 0; j < d; j++)// j is column
        {
            item = sj_array_get_nth(row, j);
            if (!item)continue;
            tile = 0;//default
            sj_get_integer_value(item, &tile);
            level->tileMap[(i * (int)level->mapSize.x) + j] = tile;
        }
    }
    sj_free(json);
    level_build(level);
    return level;
}

void level_build(Level* level) {
    int i, j;
    if (!level)return;
    if (level->tileLayer)gf2d_sprite_free(level->tileLayer);
    level->tileLayer = gf2d_sprite_new();
    if (!level->tileLayer)
    {
        slog("failed to create sprite for tileLayer");
        return;
    }
    // if there is a default surface, free it
    if (level->tileLayer->surface)SDL_FreeSurface(level->tileLayer->surface);
    //create a surface the size we need it
    //level->tileLayer->surface = gf2d_graphics_create_surface(level->tileSize.x * level->mapSize.x, level->tileSize.y * level->mapSize.y);
    level->tileLayer->surface = SDL_CreateRGBSurface(0, level->tileSize.x * level->mapSize.x, level->tileSize.y * level->mapSize.y, 32, 0, 0, 0, 0);
    if (!level->tileLayer->surface)
    {
        slog("failed to create tileLayer surface");
        return;
    }
    //make sure the surface is compatible with our graphics settings
    level->tileLayer->surface = gf2d_graphics_screen_convert(&level->tileLayer->surface);
    if (!level->tileLayer->surface)
    {
        slog("failed to create surface for tileLayer");
        return;
    }
    //draw the tile sprite to the surface
    for (j = 0; j < level->mapSize.y; j++)//j is row
    {
        for (i = 0; i < level->mapSize.x; i++)// i is column
        {
            if (level->tileMap[(j * (int)level->mapSize.x) + i] <= 0)continue;//skip zero

            //doing a check here for if == 1 or == 2 doesnt work
            //it will only load in the sprite if == 1, but never if ==2???
            /*
            if (level->tileMap[(j * (int)level->mapSize.x) + i] == 1) {
                gf2d_sprite_draw_to_surface(
                    level->tileSet,
                    vector2d(i * level->tileSize.x, j * level->tileSize.y),
                    NULL,
                    NULL,
                    level->tileMap[(j * (int)level->mapSize.x) + i] - 1,
                    level->tileLayer->surface);
            }
            if (level->tileMap[(j * (int)level->mapSize.x) + i] == 2) {
                gf2d_sprite_draw_to_surface(
                    level->tileSetAlt,
                    vector2d(i * level->tileSize.x, j * level->tileSize.y),
                    NULL,
                    NULL,
                    level->tileMap[(j * (int)level->mapSize.x) + i] - 1,
                    level->tileLayer->surface);
            }
            */
            
            gf2d_sprite_draw_to_surface(
                level->tileSetAlt,
                vector2d(i * level->tileSize.x, j * level->tileSize.y),
                NULL,
                NULL,
                level->tileMap[(j * (int)level->mapSize.x) + i] - 1,
                level->tileLayer->surface);
            
        }
    }
    //convert it to a texture
    level->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), level->tileLayer->surface);
    SDL_SetTextureBlendMode(level->tileLayer->texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(level->tileLayer->texture,
        NULL,
        level->tileLayer->surface->pixels,
        level->tileLayer->surface->pitch);
    
    level->tileLayer->frame_w = level->tileLayer->surface->w;
    level->tileLayer->frame_h = level->tileLayer->surface->h;
    level->tileLayer->frames_per_line = 1;
    level_build_static_collision_layer(level);
}

void level_build_static_collision_layer(Level* level) {
    Shape shape;
    Shape* newShape;
    int i, j;
    if ((!level) || (!level->tileLayer))return;

    for (j = 0; j < level->mapSize.y; j++)//j is row
    {
        for (i = 0; i < level->mapSize.x; i++)// i is column
        {
            if (level->tileMap[(j * (int)level->mapSize.x) + i] <= 0)continue;//skip zero
            //shape = gfc_shape_rect(i * level->tileSize.x, j * level->tileSize.y, level->tileSize.x, level->tileSize.y);
            shape = gfc_shape_rect(i * level->tileSize.x + (1200 / 2 - level->tileLayer->surface->w / 2), j * level->tileSize.y + (720 / 2 - level->tileLayer->surface->h / 2), level->tileSize.x, level->tileSize.y);

            newShape = (Shape*)malloc(sizeof(shape));
            if (!newShape)
            {
                slog("failed to allocate new space for the shape");
                return;
            }
            memcpy(newShape, &shape, sizeof(Shape));

            level->staticShapes = gfc_list_append(level->staticShapes, (void*)newShape);
        }
    }

    level_spawn_enemies(level);
}

void level_spawn_enemies(Level* level) {
    List* enemies;

    enemies = level->enemiesToSpawn;

    if (gfc_list_get_count(enemies) <= 0) return;

    for (int i = 0; i < gfc_list_get_count(enemies); i++) {
        level_add_entity(level, gfc_list_get_nth(enemies, i));
    }
}

void level_draw_static_shapes(Level* level) {
    Shape* s;

    for (int i = 0; i < level->staticShapes->count; i++) {
        s = gfc_list_get_nth(level->staticShapes, i);
        gf2d_draw_shape(*s, GFC_COLOR_YELLOW, vector2d(0, 0));
    }
}

void level_draw_active_entities_bodies(Level* level) {
    Entity* ent;

    for (int i = 0; i < level->activeEntities->count; i++) {
        ent = gfc_list_get_nth(level->activeEntities, i);
        body_draw(&ent->body, vector2d(0, 0));
    }
}

void level_add_entity(Level* level, Entity* entity) {
    level->activeEntities = gfc_list_append(level->activeEntities, entity);
    level->activeBodies = gfc_list_append(level->activeBodies, &entity->body);
}

List* level_get_active_bodies(Level* level) {
    return level->activeEntities;
}

void level_draw(Level* level)
{
    if (!level)return;
    if (!level->tileLayer)return;
    gf2d_sprite_draw_image(level->tileLayer, vector2d(1200/2 - level->tileLayer->surface->w/2, 720/2 - level->tileLayer->surface->h/2));
}

Level* level_new() {
	Level* level;
	level = gfc_allocate_array(sizeof(Level), 1);
	level->staticShapes = gfc_list_new();
    level->activeEntities = gfc_list_new();
    level->activeBodies = gfc_list_new();
    level->enemiesToSpawn = gfc_list_new();

	return level;
}

void level_free(Level* level) {
	if (!level)return;
	if (level->tileSet)gf2d_sprite_free(level->tileSet);
    if (level->tileSetAlt)gf2d_sprite_free(level->tileSetAlt);
	if (level->tileLayer)gf2d_sprite_free(level->tileLayer);
	if (level->tileMap)free(level->tileMap);
	gfc_list_foreach(level->staticShapes, free);
	gfc_list_delete(level->staticShapes);
    //gfc_list_foreach(level->activeBodies, free);
    gfc_list_delete(level->activeBodies);
    //gfc_list_foreach(level->activeEntities, free);
    gfc_list_delete(level->activeEntities);
    gfc_list_delete(level->enemiesToSpawn);
	free(level);
}


