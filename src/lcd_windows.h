#ifndef LCD_WINDOW
#define LCD_WINDOW

void set_window(u8 window_number, u8 x, u8 y, u8 width, u8 height);
void set_window_enable_io(window_enable_t flags_in, window_enable_t flags_out);

#endif /* LCD_WINDOW */
