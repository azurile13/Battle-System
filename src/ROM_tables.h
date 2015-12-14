#ifndef ROM_TABLES_H
#define ROM_TABLES_H

#include "types.h"

#define type_total 19
#define move_total 621
#define berries_table_size 67

/* most sizes are obviously inaccurate. To adjust once tables are generated */
#define natural_gift_table_size 50
#define fling_table_size 50
#define sheer_force_table_size 50
#define mega_items_table_size 50
#define iron_fist_table_size 50
#define reckless_table_size 50
#define mega_table_size 20
#define species_count 721

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

/* low priority tables: */
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


/* 
0 = normal effective
1 = super effective
2 = not effective
3 = immune
*/

struct types_chart {
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
};


u16 species_to_dex_index(u16);
extern struct move_table move[move_total];
extern struct natural_gift_table natural_gift_t[natural_gift_table_size];
extern struct fling_table fling_t[fling_table_size];
extern struct sheer_force_table sheer_force_t[sheer_force_table_size];
extern struct mega_items_table mega_items_t[mega_items_table_size];
extern struct iron_fist_table iron_fist_t[iron_fist_table_size];
extern struct reckless_table reckless_t[reckless_table_size];
extern struct berries_table berries_t[berries_table_size];
extern struct types type_chart[19];
extern struct mega_table megas[mega_items_table_size];
extern struct base_stat_entry base_stats[species_count];
extern struct dex_entry_data dex_data[species_count];
extern struct evolution_entry evolution_table[species_count];
extern struct pokemon pokemon_bank[12];
extern struct temp_vars temp_vars;
extern struct types_chart type_chart[19];

#endif /* ROM_TABLES_H */
