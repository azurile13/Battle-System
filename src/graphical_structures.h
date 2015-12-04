#ifndef GRAPHICAL_STRUCTS
#define GRAPHICAL_STRUCTS

//How should the screen be "faded" in
typedef enum
{
  BOTTOM_TOP,
  OTHER
} bs_animation_t;

//Wrapper enumeration for the gba window feature
typedef enum
{
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
} window_enable_t;

//graphic compound
typedef struct
{
  void *tileset;
  void *tilemap;
  void *palette;
} graphic_t;


#endif
