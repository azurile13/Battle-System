#ifndef LCD_H
#define LCD_H

void lcd_io_set(u8 io_register, u16 value);
void lz77_uncomp_vram(void* src, void* dst);
void pal_decompress_slice_to_faded_and_unfaded(void* src, u16 start, u16 end);
void gpu_pal_upload(void);
#endif
