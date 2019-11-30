#include <stdio.h>
#include "SwinGame.h"
#include "Game.h"

int main()
{
    open_graphics_window("Hello World", MAP_WIDTH * TILE_DIM + 300, MAP_HEIGHT * TILE_DIM);
    // show_swin_game_splash_screen();

    Game game;
    
    do
    {
        process_events();
        
        clear_screen(ColorBlack);
        
        game.update();
        game.draw();
		
		draw_framerate(0,0);
        
        refresh_screen(60);
    } while ( ! window_close_requested() );
    
    release_all_resources();
    return 0;
}
