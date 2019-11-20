#include <stdio.h>
#include "SwinGame.h"
#include "Wall.h"

int main()
{
    open_graphics_window("Hello World", 800, 600);
    show_swin_game_splash_screen();

	Wall* w = new Wall(10, 10);
    
    do
    {
        process_events();
        
        clear_screen(ColorWhite);
        
        draw_framerate(0,0);

		w->draw();
        
        refresh_screen(60);
    } while ( ! window_close_requested() );
    
    release_all_resources();
    return 0;
}
