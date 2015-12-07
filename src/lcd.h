#ifndef LCD_H
#define LCD_H

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


#endif /* LCD_H */
