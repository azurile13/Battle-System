#include "types.h"
#include "battle_engine_resource.h"
#include "ROM_tables.h"
#include "multipurpose_resources.c"
#include "defines.h"
#include "engine/battle.h"
#include "engine/variables.h"
#include "lcd.h"

void load_screen_fade(void);
void load_screen_bottom_top(void);
void battle_graphics_slide (struct battle_field *battle_field);
void test_gfx(void);
void setup(void);
void dp12_fuel(u16 current);

/* Reserved these 3 functions for SBird. */
u16 get_environment() {
	u16 custom_environment = load_var_value(VAR_BATTLE_BG_CUSTOM);
	return custom_environment > 0 ? custom_environment : get_bs_elem_env_index();
}

u8 set_up_oam_sliding() {
	// slide opponent and trainer into battle env
	return 0;
}

void clear_video()
{
    overworld_free_bgmaps();
    gpu_tile_bg_drop_all_sets(0);
	  bg_positions_reset();
    callback_clear_and_init();

    //TODO: Use DMA or memcopy instead of hacky for loops
    int i = 0;
    for(i = 0x06000000; i < 0x06010000; i+=4)
    {
        *((u32*)(i)) = 0x00000000;
    }
    for(i = 0; i < 512; i+=4)
    {
        *((u32*)(0x020371F8 + i)) = 0x00000000;
        *((u32*)(0x020375F8 + i)) = 0x00000000;
    }
}

void battle_graphics_slide (struct battle_field *battle_field) {
	clear_video();
	//Some window stuff todo
	/*lcd_io_set(0x4C, 0x00);
	lcd_io_set(0x40, 0xF0);
	lcd_io_set(0x44, 0x5051);
	lcd_io_set(0x48, 0x00);
	lcd_io_set(0x4A, 0x00);
	vblank_cb_battle_WIN0H = 0xF0;
	vblank_cb_battle_WIN0V = 0x5051;*/





	lcd_io_set(0x08, 0x9800);
	lcd_io_set(0x0A, 0x9C04);
	lcd_io_set(0x0C, 0x5E00);
	lcd_io_set(0x0E, 0x5A0B);
	lcd_io_set(0x00, 0x7F60);
	battle_load_global.battle_environment = get_environment();

	battle_load_global.animation = BOTTOM_TOP;
	battle_load_global.battle_load_state = 0;
	switch(battle_load_global.animation)
	{
		case FADE_IN:
			set_callback2(load_screen_fade);
			break;
		case BOTTOM_TOP:
			set_callback2(load_screen_bottom_top);
		  break;
	}
}

void dp12_fuel(u16 current)
{
	u16* dp12_8700_p;
	for(dp12_8700_p = &dp12_8700; dp12_8700_p < (&dp12_8700 + 0x50); dp12_8700_p++)
	{
		*(dp12_8700_p) = (u16)current;
		*(dp12_8700_p + 0x3C0) = (u16)current;
		*(dp12_8700_p + 0x50) = (u16)(0xFFFF - current + 1);
		*(dp12_8700_p + 0x410) = (u16)(0xFFFF - current + 1);
	}
}

void load_battle_background(struct bs_elements_t screen_graphic)
{
	lz77_uncomp_vram(screen_graphic.background_set, (void*)0x06008000);
	lz77_uncomp_vram(screen_graphic.background_map, (void*)0x0600d000);

	pal_decompress_slice_to_faded_and_unfaded(screen_graphic.palette, 0x20, 0x60);
}

void load_battle_accessories(struct bs_elements_t screen_graphic)
{
	lz77_uncomp_vram(screen_graphic.grass_set, (void*)0x06004000);
	lz77_uncomp_vram(screen_graphic.grass_map, (void*)0x0600E000);
}

void load_screen_bottom_top()
{
	switch(battle_load_global.battle_load_state)
	{
		case 0:
		{
			/*Honestly no idea what this is, but i'm copying the games setup for now*/
			dp12_reset_something();
			dp12_something(&BG3HOFS, (&BG3HOFS) + 4, (&BG3HOFS) + 8);

			vblank_cb_battle_BG0HOFS = 0;
			vblank_cb_battle_BG0VOFS = 0;
			vblank_cb_battle_BG1HOFS = 0;
			vblank_cb_battle_BG1VOFS = 0;
			vblank_cb_battle_BG2HOFS = 0;
			vblank_cb_battle_BG2VOFS = 0;
			vblank_cb_battle_BG3HOFS = 0xF0;
			vblank_cb_battle_BG3VOFS = 0;
			battle_load_global.revert_screen = 1;

			load_battle_background(battle_env_table[battle_load_global.battle_environment]);
			dp12_config.source_one = &dp12_8700 + 1;
			dp12_config.source_two = &dp12_8700 + 0x3C1;
			dp12_config.callback = dp12_b_callback;
			dp12_config.enable = 0x1;
			dp12_config.length = 0x1;
			dp12_config.config = 0xA260;
			battle_load_global.battle_load_state++;
		}
		break;
		case 1:
		{
			dp12_fuel(vblank_cb_battle_BG3HOFS);
			if(dp12_config.revert == 1)
			{
				dp12_config.revert = 0;
			}
			else
			{
				dp12_config.revert = 1;
			}
			(vblank_cb_battle_BG3HOFS)--;
			if(vblank_cb_battle_BG3HOFS == 0)
			{
				battle_load_global.battle_load_state++;
			}

		}
		break;
		case 2:
		{
			//finished loading, disengage dp12
			dp12_abort();
		}
	}
}

void load_screen_fade()
{
	switch(battle_load_global.battle_load_state)
	{
		case 0:
		{
			int i = 0;
			//kill the faded palettes
			for(i = 0; i < 512; i+=4)
			{
					*((u32*)(0x020375F8 + i)) = 0x00000000;
			}
			//Load gfx
			load_battle_background(battle_env_table[battle_load_global.battle_environment]);
			fade_screen(0xFFFFFFFF,0x6,0x10,0x0,0x0000);

			battle_load_global.battle_load_state = 1;
		}
		break;
		case 1:
			if(pal_fade_control.done == 0)
				battle_load_global.battle_load_state = 2;
		break;

	}
}



u8 get_ability_from_bit(struct pokemon *pokemon) {
	u8 ability = get_attr(pokemon, ATTR_ABILITY_BIT);
	u16 species = get_attr(pokemon, ATTR_SPECIES);
	if (ability) {
		return base_stats[species].ability2;
	} else {
		return base_stats[species].ability1;
	}
}

u8 get_weight(struct pokemon *pokemon) {
	u16 species = get_attr(pokemon, ATTR_SPECIES);
	species = species_to_dex_index(species);
	return dex_data[species].weight;
}

void set_types(struct battler *battler, struct pokemon *pokemon) {
	u16 species = get_attr(pokemon, ATTR_SPECIES);
	battler->types[0] = base_stats[species].type1;
	battler->types[1] = base_stats[species].type2;
	battler->types[2] = TYPE_EGG;
	return;
}

void battler_init(struct battler *battler, struct pokemon *pokemon, u8 bank) {
	battler->species = get_attr(pokemon, ATTR_SPECIES);
	battler->ability = get_ability_from_bit(pokemon);
	battler->weight = get_weight(pokemon);
	battler->level = get_attr(pokemon, ATTR_LEVEL);
	set_types(battler, pokemon);
	battler->item = get_attr(pokemon, ATTR_HELD_ITEM);
	battler->current_hp = pokemon->current_hp;
	battler->total_hp = pokemon->total_hp;
	battler->attack = pokemon->attack;
	battler->defense = pokemon->defense;
	battler->speed = pokemon->speed;
	battler->ailment = pokemon->ailment;
	battler->pokemon_bank_id = bank;
	return;
}

void battle_end (struct battle_field *battle_field) {
	// store battle outcome
	temp_vars.var_8000 = battle_field->battle_result;
	if (battle_field->battle_result != RESULT_TO_CONTINUE) {
		free(battle_field);
	}
	c2_exit_to_overworld_1_continue_scripts_and_music();
	return;
}


void do_battle (u8 task_id) {
	//struct battle_field *battle_field = &battle_data_ptrs->field_ptr;
	struct battle_field *battle_field = battle_data_ptrs.field_ptr;
	u8 counter = superstate.multi_purpose_state_tracker;
	switch (counter) {
		case 0:
			// increment turn counter
			battle_field->turn_counter ++;
			counter ++;
			break;
		case 1:
			// do text things
			// send out parties
			break;
		case 2:
			// check ability activations
			break;
		case 3:
			// player pick moves
		case 4:
			// ai pick moves
		case 5:
			// execute pre-move abilities
		case 6:
			// execute moves
		case 7:
			// update battle field. HP bars, clear dead, ect.
		case 8:
			// check battle is over -> end (captured, frontier mode loss, dex, money, revert forms, ect)
			break;
		case 9:
			// loop
			counter = 0;
			break;
		default:
			// battle is over
			task_del(task_id);
			battle_end(battle_field);
			break;
	};
	return;
}
/*
Battle (task):
	- Turn counter ++
	- Trainer was challenged/run send out anims for each side (BS)
	- player pick move to use
	- AIs pick move to use
	- execute moves -> Run "battle script" tasks for move
	- update battle field
	- check battle is over -> end (captured, frontier mode loss, dex, money, revert forms, ect)
	- give exp, learn move, evolve checks
	Back to turn counter ++
}
*/

void update() {
	task_exec();
	textbox_something();
	objc_exec();
	obj_sync_something();
	gpu_pal_upload();
	gpu_sprites_upload();
	fade_and_return_progress_probably();

	lcd_io_set(0x1C, vblank_cb_battle_BG3HOFS);
	dp12_update();
}

void setup (void) {
	superstate.multi_purpose_state_tracker = 0;
	battle_data_ptrs.task_id = task_add(do_battle, 0x1);

	vblank_handler_set(update);
	return;
}

void battle_init(struct battle_config *b_config) {
	void c2_exit_to_overworld_1_continue_scripts_and_music(void);

	// malloc resources and set up
	struct battle_field *battle_field = (struct battle_field*) malloc(sizeof(struct battle_field));
	battle_field->b_config = b_config;
	battle_field->battle_type = b_config->type;

	// set up battlers by battle type
	switch (battle_field->battle_type) {
		case SINGLE_WILD:
		case SINGLE_TRAINER:
			battle_field->active_battler_count_max = 2;
			battler_init(&battle_field->battlers[0], &pokemon_bank[6], 6); // player's first pkmn
			battler_init(&battle_field->battlers[3], &pokemon_bank[0], 0); // opponent's first pkmn
			break;
		case DOUBLE_WILD:
		case DOUBLE_TRAINER:
			battle_field->active_battler_count_max = 4;
			battler_init(&battle_field->battlers[0], &pokemon_bank[6], 6); // player's 1st pkmn
			battler_init(&battle_field->battlers[1], &pokemon_bank[7], 7); // player's 2nd pkmn
			battler_init(&battle_field->battlers[3], &pokemon_bank[0], 0); // opponent's 1st pkmn
			battler_init(&battle_field->battlers[4], &pokemon_bank[1], 1); // opponent's 2nd pkmn
			break;
		case DOUBLE_TWO_AI:
			battle_field->active_battler_count_max = 4;
			battler_init(&battle_field->battlers[0], &pokemon_bank[6], 6); // player's 1st pkmn
			battler_init(&battle_field->battlers[1], &pokemon_bank[9], 9); // ally's 1st pkmn
			battler_init(&battle_field->battlers[3], &pokemon_bank[0], 0); // opponent1's 1st pkmn
			battler_init(&battle_field->battlers[4], &pokemon_bank[3], 3); // opponent2's 2nd pkmn
			break;
		case TRIPLE_WILD:
		case TRIPLE_TRAINER:
			battle_field->active_battler_count_max = 6;
			battler_init(&battle_field->battlers[0], &pokemon_bank[6], 6); // player's 1st pkmn
			battler_init(&battle_field->battlers[1], &pokemon_bank[7], 7); // player's 2nd pkmn
			battler_init(&battle_field->battlers[2], &pokemon_bank[8], 8); // player's 3rd pkmn
			battler_init(&battle_field->battlers[3], &pokemon_bank[0], 0); // opponent's 1st pkmn
			battler_init(&battle_field->battlers[4], &pokemon_bank[1], 1); // opponent's 1st pkmn
			battler_init(&battle_field->battlers[5], &pokemon_bank[2], 2); // opponent's 2nd pkmn
			break;
		case TRIPLE_FOUR_AI: // low priority
		case TRIPLE_FIVE_AI: // low priority
			break;
		case HORDE_WILD:
		case HORDE_TRAINER:
			battle_field->active_battler_count_max = 6;
			battler_init(&battle_field->battlers[1], &pokemon_bank[6], 6); // player's 1st pkmn
			battler_init(&battle_field->battlers[0], &pokemon_bank[0], 0); // opponent's 1st pkmn
			battler_init(&battle_field->battlers[2], &pokemon_bank[1], 1); // opponent's 2nd pkmn
			battler_init(&battle_field->battlers[3], &pokemon_bank[2], 2); // opponent's 3rd pkmn
			battler_init(&battle_field->battlers[4], &pokemon_bank[1], 1); // opponent's 4th pkmn
			battler_init(&battle_field->battlers[5], &pokemon_bank[2], 2); // opponent's 5th pkmn
			break;
		default:
			free(battle_field);
			// call back, return to overworld -> safe exit
			c2_exit_to_overworld_1_continue_scripts_and_music();
			break;
	};
	battle_graphics_slide(battle_field);
	setup();
	return;
}
