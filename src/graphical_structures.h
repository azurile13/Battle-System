#ifndef GRAPHICAL_STRUCTS
#define GRAPHICAL_STRUCTS

#include "types.h"

//How should the screen be "faded" in
typedef enum
{
  BOTTOM_TOP,
  FADE_IN
} bs_animation_t;

//Battlescreen elements used to draw the screen
typedef struct
{
    void *background_set;
    void *background_map;
    void *grass_set;
    void *grass_map;
    void *palette;
} bs_elements_t;

typedef struct
{
    u8 battle_load_state;
} bs_load_var_t;

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

typedef struct
{
    u16 bg_bitfield;
    u16 obj_bitfield;
    u16 scale;
    u16 mix_color;

    u8 done;
    u8 field9;
    u8 fieldA;
    u8 fieldB;
} fade_control_t;

//graphic compound
typedef struct
{
  void *tileset;
  void *tilemap;
  void *palette;
} graphic_t;


#endif
