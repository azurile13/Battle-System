#ifndef LCD_H
#define LCD_H

struct fade_control_t {
    u16 bg_bitfield;
    u16 obj_bitfield;
    u16 scale;
    u16 mix_color;

    u8 done;
    u8 field9;
    u8 fieldA;
    u8 fieldB;
};

void lcd_io_set(u8 io_register, u16 value);
void lz77_uncomp_vram(void* src, void* dst);
void pal_decompress_slice_to_faded_and_unfaded(void* src, u16 start, u16 end);
void gpu_pal_upload(void);
void overworld_free_bgmaps(void);
void gpu_tile_bg_drop_all_sets(u8);
void bg_positions_reset(void);
void bgid_nullify_tilemap(u8 bg);
void copy_queue_process(void);
void gpu_sprites_upload(void);
u16 lcd_io_get(u8 io_register);
u8 fade_screen(u32 bitmask, s8 speed, u8 to, u8 from, u16 color);
void fade_and_return_progress_probably(void);
void bg_scroll_invert(void);

extern struct fade_control_t pal_fade_control;

extern u16 BG3HOFS;
extern u32 DMA0_SRC;
extern u32 DMA0_DST;
extern u16 DMA0_CNT_H;
extern u16 DMA0_CNT_L;

#endif /* LCD_H */
