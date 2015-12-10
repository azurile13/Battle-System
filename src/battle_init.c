#include "types.h"
#include "battle_engine_resource.h"
#include "ROM_tables.h"
#include "multipurpose_resources.c"
#include "defines.h"


/* Reserved these 3 functions for SBird. */
u8 get_environment() {
	// tile standing on & map & table look up
	return 0;
}

u8 set_up_oam_sliding() {
	// slide opponent and trainer into battle env
	return 0;
}

void battle_graphics_slide (struct battle_field *battle_field) {
	return;
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
		free (battle_field);
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

void update(u8 task_id) {
	task_exec();
	textbox_something();
	objc_exec();
	obj_sync_something();
	fade_and_return_progress_probably();
	do_battle(task_id);
	return;
}

void setup (void) {
	superstate.multi_purpose_state_tracker = 0;
	battle_data_ptrs.task_id = task_add(update, 0x1);
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


