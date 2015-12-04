#include "graphical_structures.h"
#include "types.h"
#include "lcd_windows.h"
#include "lcd.h"
#include "callback.h"

void load_screen(void);
void pre_init(void);
void wait_init(void);
void init_battle_screen(bs_elements_t screen_graphic, bs_animation_t animation);


void load_screen()
{
    hblank_handler_set(0);
    vblank_handler_set(0);
    set_callback2(pre_init);
}

void pre_init()
{
    lcd_io_set(0x08, 0x9800);
    lcd_io_set(0x0A, 0x9C00);
    lcd_io_set(0x0C, 0x5E00);
    lcd_io_set(0x0E, 0x5A0B);
    bs_elements_t elements = (*((bs_elements_t*)(0x0824EE34)));
    init_battle_screen(elements, BOTTOM_TOP);
    set_callback2(wait_init);
}

void wait_init()
{
    task_exec();
    objc_exec();
    obj_sync_something();
    gpu_pal_upload();
}

void init_battle_screen(bs_elements_t screen_graphic, bs_animation_t animation)
{
    //Just load screen elements
    lz77_uncomp_vram(screen_graphic.background_set, (void*)0x06008000);
    lz77_uncomp_vram(screen_graphic.background_map, (void*)0x0600d000);

    pal_decompress_slice_to_faded_and_unfaded(screen_graphic.palette, 0x20, 0x60);
    //set the hblank callback to load window stuff
}
