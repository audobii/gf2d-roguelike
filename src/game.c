#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"

#include "gfc_shape.h"
#include "gfc_input.h"

#include "level.h"
#include "entity.h"

#include "slime.h"
#include "enemy_bigger_slime.h"
#include "enemy_sentient_drill.h"
#include "enemy_crystal_guy.h"
#include "enemy_glowfly.h"
#include "enemy_rock_block.h"

#include "pickup.h"
#include "shop_item.h"

#include "player.h"
#include "gf2d_collision.h"

#include "gf2d_mouse.h"

#include "cave_hole.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    Entity* ent;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,255,255,180);
    
    Level* level;

    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_manager_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

    gfc_input_init("config/input.json");
    
    /*demo setup*/
    //game window is 1200x720
    sprite = gf2d_sprite_load_image("images/backgrounds/floor1.png");
    mouse = gf2d_sprite_load_image("images/cursor.png");
    //ent = rock_block_new(vector2d(800,250));
    //Entity* ent2 = rock_block_new(vector2d(300, 400));

    level = level_load("rooms/startRoom.json");
    level_set_active_level(level);

    //SPAWN PLAYER AT 600, 555
    player_new(vector2d(600, 555));
    cave_hole_new(vector2d(600, 200));

    //TEMPORARY 
    //TODO: please add level_add_entity to every _new function for enemies and player
    level_add_entity(level, player_get());
    //level_add_entity(level, ent);
    //level_add_entity(level, ent2);

    List* collisions = gfc_list_new();

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

        gfc_input_update();

        entity_think_all();
        entity_update_all();

        gf2d_collision_build_list(collisions, level_get_active_level()->staticShapes, level_get_active_level()->activeBodies); 
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            level_draw(level_get_active_level());

            entity_draw_all();
            
            level_draw_static_shapes(level_get_active_level());

            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                0 //(int)mf
                );

            player_draw_hud(player_get());

            gf2d_collision_update(collisions);
        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    entity_free_all();
    level_free(level_get_active_level());
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
