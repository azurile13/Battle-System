#include "types.h"
#include "lcd_windows.h"
#include "lcd.h"

void set_window(u8 window_number, u8 x, u8 y, u8 width, u8 height) {
    u8 right_x = x + width - 1;
    u8 bottom_y = y + height - 1;
    u16 horizontal = (x << 8) | right_x;
    u16 vertical = (y << 8) | bottom_y;
    if (window_number == 0) {
        lcd_io_set(0x40, horizontal);
        lcd_io_set(0x44, vertical);
    } else {
        lcd_io_set(0x42, horizontal);
        lcd_io_set(0x46, vertical);
    }
}

void set_window_enable_io(enum window_enable_t flags_in, enum window_enable_t flags_out) {
    lcd_io_set(0x48, flags_in);
    lcd_io_set(0x4A, flags_out);
}
