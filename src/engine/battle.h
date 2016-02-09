#ifndef ENGINE_BATTLE_H_
#define ENGINE_BATTLE_H_

/*Use map_temp var for custom backgrounds*/
#define VAR_BATTLE_BG_CUSTOM 0x4000
#define BATTLE_ENV_TABLE_SIZE 0x10 
/*Not sure about the actual size*/

typedef void (*dp12_callback)(void);

u8 get_bs_elem_env_index();
void dp12_reset_something(void);
void dp12_something(u16* lcd1, u16* lcd2, u16* lcd3);
void dp12_update(void);
void dp12_b_callback(void);
void dp12_abort(void);

struct dp12_struct {
  void* source_one;
  void* source_two;
  void* destination;
  u16 length;
  u16 config;
  dp12_callback callback;
  u8 revert;
  u8 enable;
  u16 field_16;
};

struct bs_elements_t {
    void *background_set;
    void *background_map;
    void *grass_set;
    void *grass_map;
    void *palette;
};

enum bs_animation_t {
  BOTTOM_TOP,
  FADE_IN
};

struct bs_load_var_t
{
    u8 battle_load_state;
    enum bs_animation_t animation;
    u16 battle_environment;
    u8 revert_screen;
    u16 bg3_hof;
};

/*
struct dp12_2038700 {
	u16 bank[0x4F];
};

struct dp12_big_2038700 {
	u16 bank[0x4F];
};*/



extern struct bs_elements_t battle_env_table[BATTLE_ENV_TABLE_SIZE];
extern struct bs_load_var_t battle_load_global;
extern struct dp12_struct dp12_config;

extern u16 dp12_8700;

extern u16 vblank_cb_battle_BG0HOFS;
extern u16 vblank_cb_battle_BG0VOFS;
extern u16 vblank_cb_battle_BG1HOFS;
extern u16 vblank_cb_battle_BG1VOFS;
extern u16 vblank_cb_battle_BG2HOFS;
extern u16 vblank_cb_battle_BG2VOFS;
extern u16 vblank_cb_battle_BG3HOFS;
extern u16 vblank_cb_battle_BG3VOFS;
extern u16 vblank_cb_battle_WIN0H;
extern u16 vblank_cb_battle_WIN0V;
extern u16 vblank_cb_battle_WIN1H;
extern u16 vblank_cb_battle_WIN1V;


#endif /*ENGINE_BATTLE_H_*/
