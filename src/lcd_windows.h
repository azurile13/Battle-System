#ifndef LCD_WINDOW
#define LCD_WINDOW

enum window_enable_t {
    W0_BG0 = 1 << 0,
    W0_BG1 = 1 << 1,
    W0_BG2 = 1 << 2,
    W0_BG3 = 1 << 3,

    W0_OBJ = 1 << 4,
    W0_SFX = 1 << 5,

    W1_BG0 = 1 << 8,
    W1_BG1 = 1 << 9,
    W1_BG2 = 1 << 10,
    W1_BG3 = 1 << 11,

    W1_OBJ = 1 << 12,
    W1_SFX = 1 << 13
};

void set_window(u8 window_number, u8 x, u8 y, u8 width, u8 height);
void set_window_enable_io(enum window_enable_t flags_in, enum window_enable_t flags_out);

#endif /* LCD_WINDOW */
