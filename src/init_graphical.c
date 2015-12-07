#include "graphical_structures.h"
#include "types.h"
#include "lcd_windows.h"
#include "lcd.h"
#include "callback.h"

void load_screen(void); //__attribute__ ((section (".init")));;
void pre_init(void);
void wait_init(void);
void clear_video(void);
void init_battle_screen(bs_elements_t screen_graphic, bs_animation_t animation);
void load_screen_open_bottom_top(void);
void load_screen_fade(void);

bs_load_var_t* bs_globals = (bs_load_var_t*)((void*)0x0203F000);
fade_control_t* fade_control = (fade_control_t*)((void*)0x02037AB8);

void load_screen()
{
    hblank_handler_set(0);
    vblank_handler_set(0);
    set_callback2(pre_init);
}

void pre_init()
{
    clear_video();
    lcd_io_set(0x08, 0x9800);
    lcd_io_set(0x0A, 0x9C00);
    lcd_io_set(0x0C, 0x5E00);
    lcd_io_set(0x0E, 0x5A0B);
    lcd_io_set(0x00, 0x7F60);

    //TODO: Load bs_elements_t from environment
    bs_elements_t elements = (*((bs_elements_t*)(0x0824EE34)));
    init_battle_screen(elements, FADE_IN);
    vblank_handler_set(wait_init);
}

void clear_video()
{
    overworld_free_bgmaps();
    gpu_tile_bg_drop_all_sets(0);
	bg_positions_reset();
    callback_clear_and_init();

    //TODO: Use DMA or memcopy instead of hacky for loops
    int i = 0;
    for(i = 0x06000000; i < 0x06010000; i+=4)
    {
        *((u32*)(i)) = 0x00000000;
    }
    for(i = 0; i < 512; i+=4)
    {
        *((u32*)(0x020371F8 + i)) = 0x00000000;
        *((u32*)(0x020375F8 + i)) = 0x00000000;
    }
}

void load_screen_open_bottom_top()
{
    //TODO: Add mirrored bg stuff
    u16 bg3h = lcd_io_get(0x1C);
    if(bg3h > 0)
        lcd_io_set(0x1C, (u16)(bg3h - 1));
}

void load_screen_fade()
{
    if(bs_globals->battle_load_state == 0)
    {
        int i = 0;
        //kill the faded palettes
        for(i = 0; i < 512; i+=4)
        {
            *((u32*)(0x020375F8 + i)) = 0x00000000;
        }
        bs_globals->battle_load_state = 1;
    }
    else if(bs_globals->battle_load_state == 1)
    {
        fade_screen(0xFFFFFFFF,0x6,0x10,0x0,0x0000);
        bs_globals->battle_load_state = 2;
    }
    else if(bs_globals->battle_load_state == 2)
    {
        if(fade_control->done == 0)
            bs_globals->battle_load_state = 3;
    }
    else if(bs_globals->battle_load_state == 3)
    {
        //finished loading screen
    }

}

void wait_init()
{
    task_exec();
    objc_exec();
    obj_sync_something();
    gpu_pal_upload();
    gpu_sprites_upload();
    fade_and_return_progress_probably();
}

void init_battle_screen(bs_elements_t screen_graphic, bs_animation_t animation)
{
    //Just load screen elements
    lz77_uncomp_vram(screen_graphic.background_set, (void*)0x06008000);
    lz77_uncomp_vram(screen_graphic.background_map, (void*)0x0600d000);

    pal_decompress_slice_to_faded_and_unfaded(screen_graphic.palette, 0x20, 0x60);
    //set the hblank callback to load window stuff

    switch (animation)
    {
        case FADE_IN:
            set_callback2(load_screen_fade);
            break;
        case BOTTOM_TOP:
            set_callback2(load_screen_open_bottom_top);
            break;
    }

}
