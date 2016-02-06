#ifndef ROM_TABLES_H
#define ROM_TABLES_H

#include "types.h"
#include "engine/objects.h"
#include "engine/callback.h"

/* 
 * Table size constants
 */
#define type_total 19
#define move_total 621
#define berries_table_size 67

// Inaccurate sizes. Must adjust.
#define natural_gift_table_size 50
#define fling_table_size 50
#define sheer_force_table_size 50
#define mega_items_table_size 50
#define iron_fist_table_size 50
#define reckless_table_size 50
#define mega_table_size 20
#define species_count 721



/*
 *	RAM Structs and tables
 */

 
 struct pokemon {
	u32 PID;
	u32 OTID;
	u8 name[10];
	u16 language;
	u8 OT_name[7];
	u8 markings;
	u16 checksum;
	u16 padding_maybe;
	u8 data[48];
	u32 ailment;
	u8 level;
	u8 pokerus;
	u16 current_hp;
	u16 total_hp;
	u16 attack;
	u16 defense;
	u16 speed;
	u16 sp_attack;
	u16 sp_defense;
};

struct textflags {
	u8 unknown[4];// No idea, but controls text continue
};

struct saveblock_trainerdata {
	struct sav2 *sav2;
};

struct sav2 {
	u8 name[8];
	u8 gender;
	u8 padding;
	u16 trainer_id;
	u16 secret_id;
	u16 hours;
	u8 minutes;
	u8 seconds;
	u8 frames;
	u8 options_button_style;
	u8 options_text_speed;
	u8 options_battle_style[7];
	u32 field_1C;
	u8 field_20[8];
	u8 pokemon_flags_3[52];
	u8 pokemon_flags_4[52];
	u8 field_90[1044];
	u8 fourCharacters[4];
	u8 field_4A8[1008];
	u8 mapdata[1672];
	u32 bag_item_quantity_xor_value;
	u8 field_F24[127];
	u8 last_byte_in_sav2;
};


struct temp_vars {
	// Only 0x8000s here
	u16 var_8000;
	u16 var_8001;
	u16 var_8002;
	u16 var_8003;
	u16 var_8004;
	u16 var_8005;
	u16 var_8006;
	u16 var_8007;
	u16 var_8008;
	u16 var_8009;
	u16 var_800A;
	u16 var_800B;
	u16 var_800D;
	u16 var_800F;
	u16 var_800C;
	u16 var_8010;
	u16 var_8011;
};

 struct super {
	super_callback callback1;
	super_callback callback2;
	super_callback callback2backup;
	super_callback callback5_vblank;
	super_callback hblank_callback;
	u32 field_14;
	u32 field_18;
	u32 bit_to_wait_for;
	u32 *ptr_vblank_counter;
	u32 field_24;
	u16 buttons_held;
	u16 buttons_new;
	u16 buttons_held_remapped;
	u16 buttons_new_remapped;
	u16 buttons_new_and_key_repeat;
	u32 keypad_countdown;
	u32 unused_padding;
	struct sprite sprites[128];
	u8 multi_purpose_state_tracker;
	u8 gpu_sprites_upload_skip;
 };
 
extern struct super superstate;

extern struct pokemon pokemon_bank[12];
extern struct saveblock_trainerdata saveblock2;
extern struct textflags textflags;
extern struct temp_vars temp_vars;

extern struct objtemplate objt_pokemon[4];
extern struct objtemplate template_0203AD40;

 
 
 
/*
 * ROM Graphic Structs and Tables
 */
 

typedef struct pal_table {
		u8 *pal_off;
		u8 id;
		u8 filler[3];
} pal_table;


typedef struct gfx_img {
	u8 *ptr_img;
	u16 size;
	u16 index;
} gfx_img;



struct trainer_animations {
	struct frame* enter_field[2];
	struct frame* throw_ball[6];
};


struct meta_animtable_trainer {
	struct frame** trainer_anim[6];
};


struct animtable1_front {
	struct frame **  animations_front[0x8F];
};


struct animtable1_pokemon {
	struct frame ** anim[4];
};



extern gfx_img gfx_table_pokemon_back[0x19C];
extern gfx_img gfx_table_pokemon_front[0x19C];
extern gfx_img gfx_table_trainer_front[6];

extern pal_table pal_table_nonshiny[0x19C];
extern pal_table pal_table_shiny[0x19C];
extern pal_table pal_table_trainer_front[0x94];
extern pal_table pal_table_trainer_back[6];

extern struct trainer_animations trainer_animations;
extern struct meta_animtable_trainer meta_animtable_trainer;
extern struct animtable1_pokemon animtable1_pokemon;
extern struct animtable1_front animtable1_front;
extern struct objtemplate ball_templates[12];


/*
 * ROM Data tables
 */
 
 
struct trainer {
	u8 custom_items : 1;
	u8 custom_attacks : 1;
	u8 padding1 : 6; // unknown, maybe padding.
	u8 trainer_class;
	u8 gender : 1;
	u8 intro_music : 7;
	u8 trainer_pic;
	u8 trainer_name[13];
	u16 trainer_items[4];
	u8 unk1;
	u8 double_battle;
	u8 unk2[3]; // maybe padding - doubtful though
	u8 trainer_AI_difficulty;
	u8 unk3[3]; // maybe padding - doubtful though
	u8 poke_count;
	u8 padding[3];
	union {
		struct pokemon_battle_template_3 *pkmn; // fully customized
		struct pokemon_battle_template_2 *pkmn1; // moves customized
		struct pokemon_battle_template_1 *pkmn2; // items customized
		struct pokemon_battle_template_0 *pkmn3; // no customizations
	};
};


struct evolution_entry {
	u8 method;
	u16 condition;
	u16 species;
	u8 padding[3];
};


struct dex_entry_data {
	u8 name[12];
	u16 height;
	u16 weight;
	u32 description_ptr_1;
	u32 description_ptr_2;
	u16 pkmn_scale;
	u16 pkmn_offset;
	u16 trainer_scale;
	u16 trainer_offset;
};


struct base_stat_entry {
	u8 hp;
	u8 attack;
	u8 defense;
	u8 speed;
	u8 sp_attack;
	u8 sp_def;
	u8 type1;
	u8 type2;
	u8 catch_rate;
	u8 exp_yield;
	u16 effort_yield;
	u16 item_1;
	u16 item_2;
	u8 gender_chance;
	u8 step_to_hatch;
	u8 base_friendship;
	u8 exp_point_table_nr;
	u8 egg_group1;
	u8 egg_group2;
	u8 ability1;
	u8 ability2;
	u8 safari_flee_rate;
	u8 color;
	u16 padding;
};


struct move_table {
	u8 script_id;
	u8 base_power;
	u8 type;
	u8 accuracy;
	u8 PP;
	u8 effect_accuracy;
	u8 affected_targets;
	s8 priority;
	u8 contact : 1;
	u8 protect : 1;
	u8 magic_coat : 1;
	u8 snatch : 1;
	u8 mirror_move : 1;
	u8 kings_2,k : 1;
	u8 crit_chance : 1;
	u8 padding_field : 1;
	u8 is_special;
	u16 move_id;
};

// to optimize.
struct natural_gift_table {
	u16 item_id;
};


struct fling_table {
	u16 move_id;
	u8 base_power;
};


struct sheer_force_table {
	u16 move_id;
};


struct mega_items_table {
	u16 item_id;
};


struct berries_table {
	u16 item_id;
};


struct iron_fist_table {
	u16 move_id;
};


struct reckless_table {
	u16 move_id;
};


struct mold_breaker_table {
	u8 ability_id;
};


struct types {
	u8 something;
};

struct mega_table {
	u16 species;
	u16 item;
};


/* 
0 = normal effective
1 = super effective
2 = not effective
3 = immune
*/

struct types_chart {
	/*
	u8 normal_weakness;
	u8 fighting_weakness;
	u8 flying_weakness;
	u8 poison_weakness;
	u8 ground_weakness;
	u8 rock_weakness;
	u8 bug_weakness;
	u8 ghost_weakness;
	u8 steel_weakness;
	u8 egg_weakness;
	u8 fire_weakness;
	u8 water_weakness;
	u8 grass_weakness;
	u8 electric_weakness;
	u8 psychic_weakness;
	u8 ice_weakness;
	u8 dragon_weakness;
	u8 dark_weakness;
	u8 fairy_weakness;
	*/
	u8 weakness[19];
};

struct battle_strings {
	u8 *string[50]; // to add as more come out
};

extern struct objtemplate uns_table_pokemon_trainer[6];
extern struct objtemplate uns_table_pokemon_real[4];

extern struct dword_8239F8C dword_8239F8C;

extern struct move_table move[move_total];
extern struct natural_gift_table natural_gift_t[natural_gift_table_size];
extern struct fling_table fling_t[fling_table_size];
extern struct sheer_force_table sheer_force_t[sheer_force_table_size];
extern struct mega_items_table mega_items_t[mega_items_table_size];
extern struct iron_fist_table iron_fist_t[iron_fist_table_size];
extern struct reckless_table reckless_t[reckless_table_size];
extern struct berries_table berries_t[berries_table_size];
extern struct mega_table megas[mega_items_table_size];
extern struct base_stat_entry base_stats[species_count];
extern struct dex_entry_data dex_data[species_count];
extern struct evolution_entry evolution_table[species_count];

extern struct types_chart type_chart[19];

extern struct battle_strings battle_strings;

extern void *memtest;

#endif /* ROM_TABLES_H */
