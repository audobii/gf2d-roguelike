#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    Sprite* test_sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Color mouseColor = gfc_color8(255,255,255,180);

    Sprite* test_surface;
    SDL_Surface* surface = SDL_CreateRGBSurface(0, 1200, 700, 32, 0, 0, 0, 0);
    
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
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    //game window is 1200x720
    sprite = gf2d_sprite_load_image("images/backgrounds/floor1.png");
    mouse = gf2d_sprite_load_image("images/cursor.png");
    test_sprite = gf2d_sprite_load_all("images/wall1.png",64,64,1,true);

    //create another sprite - tileLayer
    test_surface = gf2d_sprite_new();
    test_surface->frame_h = 500;
    test_surface->frame_w = 500;
    test_surface->frames_per_line = 1;
    //make sure any default surface is freed
    if (test_surface->surface)SDL_FreeSurface(test_surface->surface);
    //put new surface
    test_surface->surface = surface;
    test_surface->surface = gf2d_graphics_screen_convert(&test_surface->surface);

    //draw tiles 
    gf2d_sprite_draw_to_surface(test_sprite, vector2d(0, 0), NULL, NULL, 0, test_surface->surface);

    //convert to texture
    test_surface->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), test_surface->surface);
    SDL_SetTextureBlendMode(test_surface->texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(test_surface->texture,
        NULL,
        test_surface->surface->pixels,
        test_surface->surface->pitch);

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));

            //texture becomes null here for some reason?? - this doesnt work
            gf2d_sprite_draw_image(test_surface, vector2d(350, 100));

            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                0
                //(int)mf
                );

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
