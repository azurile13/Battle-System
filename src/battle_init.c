#include "types.h"
#include "battle_engine_resource.h"
#include "ROM_tables.h"
#include "multipurpose_resources.h"
#include "defines.h"
#include "engine/battle.h"
#include "engine/variables.h"
#include "lcd.h"
#include "textbox.c"
#include "engine/objects.h"
#include "engine/callback.h"
#include "enter_battle.c"
#include "engine/rom_funcs.h"
#include "strings.h"
#include "decoder.c"


void obj_and_aux_reset_all(void);
void gpu_pal_allocator_reset(void);
void tasks_init(void);


static u8 *idx_for_battle_screen_elements_by_env = (u8 *)0x02022B50;
static u16 c_x5051 = 0x5051;
static u32 unk_05006000 = 0x5006000;
static u16* dp12_2038700 = (u16 *)0x02038700;
static u16* dp12_big_2038700 = (u16 *)(0x02038700 + 0x780);
static u16* dp12_big1_2038700 = (u16 *)(0x02038700 + 0x820);
static u16* dp12_big2_2038700 = (u16 *)(0x02038700 + 0xA0);
static u32 c_x0400001C = 0x0400001C;
static u32 c_xA2600001 = 0xA2600001;
static u8 *gpu_pal_tag_search_lower_boundary = (u8 *)0x03003E58;

void sub_8087EE4(u32, u32, u8);
void pal_fade_control_and_dead_struct_reset(void);
void sub_0800F34C(void);
void sub_0800F420(void);
void vblank_cb_battle(void);
void load_battle_screen_elements2(u8);
void *malloc_and_clear(u32);




void name_by_species_to_buffer(u16 species, u8 buff_id) {
	switch (buff_id) {
		case 1:
			{
			str_cpy(buffer1, pkmn_names_table[species].name);
			break;
			}
		case 2:
			{
			str_cpy(buffer2, pkmn_names_table[species].name);
			break;
			}
		case 3:
			{
			str_cpy(buffer3, pkmn_names_table[species].name);
			break;
			}
		default:
			{
			str_cpy(buffer4, pkmn_names_table[species].name);
			break;
			}
	};
	
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
		free(battle_field);
	}
	c2_exit_to_overworld_1_continue_scripts_and_music();
	return;
}


void battle_prep_second_stage() {
	void remoboxes_upload_tilesets(void);
	objc_exec();
	obj_sync_something();
	gpu_pal_upload();
	gpu_sprites_upload();
	remoboxes_upload_tilesets();
	fade_and_return_progress_probably();
	task_exec();
	copy_queue_process();
}


static u8* battle_type_flags = (u8 *)0x02022B4C;
u8 battle_load_something(u8*, u8*);


void battle_intro() {

	struct battle_field *battle_field = battle_mallocd_resources.battle_field;
	switch (superstate.multi_purpose_state_tracker) {
		case 0:
		{
			// load sliding player side first 
			
			u8 gender = saveblock2.sav2->gender;
			if (battle_field->battle_type == DOUBLE_TWO_AI) {
			
				u8 id = oam_trainer_back(gender, PLAYER1_OAM_START_POS_X, PLAYER1_OAM_START_POS_Y, (object_callback) obj_callback_slide_left);
				objects[id].private[1] = 2;
				objects[id].private[2] = 0x20;
				battle_mallocd_resources.ids_in_use[1] = id;
				
				id = oam_trainer_back(battle_field->b_config->ally_backsprite, PLAYER1_OAM_START_POS_X, PLAYER1_OAM_START_POS_Y, (object_callback) obj_callback_slide_left);
				objects[id].private[1] = 1;
				objects[id].private[2] = 0x55;
				battle_mallocd_resources.ids_in_use[0] = id;
				
			} else {
			
				u8 id =	oam_trainer_back(gender, PLAYER1_OAM_START_POS_X, PLAYER1_OAM_START_POS_Y, (object_callback) obj_callback_slide_left);
				objects[id].private[1] = 1;
				objects[id].private[2] = 0x20;
				battle_mallocd_resources.ids_in_use[1] = id;
				battle_mallocd_resources.ids_in_use[0] = 0x7F; //special flag
			}
			
			// load sliding opponent side
			switch (battle_field->battle_type) {
			case SINGLE_TRAINER:
			case DOUBLE_TRAINER: // one trainer, two pkmn
				{
				u8 id = oam_trainer_front(battle_field->b_config->opp_id[0], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
				objects[id].private[0] = 0x20;
				objects[id].private[1] = 1;
				battle_mallocd_resources.ids_in_use[2] = id;
				battle_mallocd_resources.ids_in_use[3] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[4] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[5] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[6] = 0x7F; //special flag
				decoder((char *)ca_trainer_single_intro);
				break;
				}
			case DOUBLE_WILD:
				{
				u8 id = oam_pkmn_front(battle_field->b_config->opp_id[0], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
				objects[id].private[0] = 0x20;
				objects[id].private[1] = 1;
				battle_mallocd_resources.ids_in_use[3] = id;
				id = oam_pkmn_front(battle_field->b_config->opp_id[1], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
				objects[id].private[0] = 0x50;
				objects[id].private[1] = 2;
				battle_mallocd_resources.ids_in_use[2] = id;
				battle_mallocd_resources.ids_in_use[4] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[5] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[6] = 0x7F; //special flag
				decoder((char *)ca_wild_double_intro);
				break;
				}
			case DOUBLE_TWO_AI:
				{
				u8 id = oam_trainer_front(battle_field->b_config->opp_id[0], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
				objects[id].private[0] = 0x20;
				objects[id].private[1] = 1;
				battle_mallocd_resources.ids_in_use[3] = id;
				id = oam_trainer_front(battle_field->b_config->opp_id[1], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
				objects[id].private[0] = 0x50;
				objects[id].private[1] = 2;
				battle_mallocd_resources.ids_in_use[2] = id;
				battle_mallocd_resources.ids_in_use[4] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[5] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[6] = 0x7F; //special flag
				decoder((char *)ca_trainer_double_intro);
				break;
				}
			case TRIPLE_WILD:
				{
				u8 id = oam_pkmn_front(battle_field->b_config->opp_id[0], 0, OPPONENT1_OAM_START_POS_X - 20, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
				objects[id].private[0] = 0x20;
				objects[id].private[1] = 1;
				battle_mallocd_resources.ids_in_use[4] = id;
				id = oam_pkmn_front(battle_field->b_config->opp_id[1], 0, OPPONENT1_OAM_START_POS_X - 30, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
				objects[id].private[0] = 0x50;
				objects[id].private[1] = 2;
				battle_mallocd_resources.ids_in_use[3] = id;
				id = oam_pkmn_front(battle_field->b_config->opp_id[2], 0, OPPONENT1_OAM_START_POS_X - 40, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
				objects[id].private[0] = 0x80;
				objects[id].private[1] = 3;
				battle_mallocd_resources.ids_in_use[2] = id;
				battle_mallocd_resources.ids_in_use[5] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[6] = 0x7F; //special flag
				decoder((char *)ca_wild_triple_intro);
				break;
				}
			case TRIPLE_TRAINER:
				{
				u8 id = oam_trainer_front(battle_field->b_config->opp_id[0], OPPONENT1_OAM_START_POS_X - 20, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
				objects[id].private[0] = 0x20;
				objects[id].private[1] = 6;
				battle_mallocd_resources.ids_in_use[4] = id;
				id = oam_trainer_front(battle_field->b_config->opp_id[1], OPPONENT1_OAM_START_POS_X - 25, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
				objects[id].private[0] = 0x40;
				objects[id].private[1] = 7;
				battle_mallocd_resources.ids_in_use[3] = id;
				id = oam_trainer_front(battle_field->b_config->opp_id[2], OPPONENT1_OAM_START_POS_X - 0x2F, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
				objects[id].private[0] = 0x60;
				objects[id].private[1] = 8;
				battle_mallocd_resources.ids_in_use[2] = id;
				battle_mallocd_resources.ids_in_use[5] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[6] = 0x7F; //special flag
				decoder((char *)ca_trainer_triple_intro);
				break;
				}
			case HORDE_WILD:
				{
				u8 id = oam_pkmn_front(battle_field->b_config->opp_id[0], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
				objects[id].private[0] = 0x30;
				battle_mallocd_resources.ids_in_use[6] = id;
				id = oam_pkmn_front(battle_field->b_config->opp_id[1], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
				objects[id].private[0] = 0x50;
				battle_mallocd_resources.ids_in_use[5] = id;
				id = oam_pkmn_front(battle_field->b_config->opp_id[2], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
				objects[id].private[0] = 0x70;
				battle_mallocd_resources.ids_in_use[4] = id;
				id = oam_pkmn_front(battle_field->b_config->opp_id[3], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
				objects[id].private[0] = 0x90;
				battle_mallocd_resources.ids_in_use[3] = id;
				id = oam_pkmn_front(battle_field->b_config->opp_id[4], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
				objects[id].private[0] = 0xA0;
				battle_mallocd_resources.ids_in_use[2] = id;
				decoder((char *)ca_wild_horde_intro);
				break;
				}
			case HORDE_TRAINER:
				{
				u8 id = oam_trainer_front(battle_field->b_config->opp_id[0], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
				objects[id].private[0] = 0x30;
				battle_mallocd_resources.ids_in_use[6] = id;
				id = oam_trainer_front(battle_field->b_config->opp_id[1], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
				objects[id].private[0] = 0x50;
				battle_mallocd_resources.ids_in_use[5] = id;
				id = oam_trainer_front(battle_field->b_config->opp_id[2], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
				objects[id].private[0] = 0x70;
				battle_mallocd_resources.ids_in_use[4] = id;
				id = oam_trainer_front(battle_field->b_config->opp_id[3], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
				objects[id].private[0] = 0x90;
				battle_mallocd_resources.ids_in_use[3] = id;
				id = oam_trainer_front(battle_field->b_config->opp_id[4], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
				objects[id].private[0] = 0xA0;
				battle_mallocd_resources.ids_in_use[2] = id;
				decoder((char *)ca_trainer_horde_intro);
				break;
				}
			case SINGLE_WILD:
			default:
				{
				u8 id = oam_pkmn_front(battle_field->b_config->opp_id[0], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
				objects[id].private[0] = 0x20;
				objects[id].private[1] = 6;
				battle_mallocd_resources.ids_in_use[2] = id;
				battle_mallocd_resources.ids_in_use[3] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[4] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[5] = 0x7F; //special flag
				battle_mallocd_resources.ids_in_use[6] = 0x7F; //special flag
				decoder((char *)ca_wild_single_intro);
				break;
				}
			};
			
			superstate.multi_purpose_state_tracker = 1;
			break;
		}
		case 1: 
		{
			if (superstate.multi_purpose_state_tracker == 1) {
				// wait for trainers to be in positions
				u8 t_1_player = battle_mallocd_resources.ids_in_use[1];
				u8 t_1_opponent = battle_mallocd_resources.ids_in_use[2];
				if ((objects[t_1_opponent].private[0] == 2) && (objects[t_1_player].private[0] == 2)) {
					textbox_set_text(string_buffer, 1, 0, 1, 3, 1);
					task_add(waitbutton_text, 1);
					superstate.multi_purpose_state_tracker = 2;
				}

			}
		break;
		}
		case 3:
			// intro text
			// if against wild, player sends out pkmn
			{
			u16 species = 0;
			if (battle_field->battle_type > HORDE_WILD) {
				switch (battle_field->battle_type) {
					case SINGLE_TRAINER:
						{
						// pkmn 1
						species = battle_field->battlers[3].species;
						name_by_species_to_buffer(species, 1);
						decoder((char*)ca_trainer_single_sentout);
						break;
						}
					case DOUBLE_TRAINER:
						{
						// first
						species = battle_field->battlers[3].species;
						name_by_species_to_buffer(species, 1);
						//second
						species = battle_field->battlers[4].species;
						name_by_species_to_buffer(species, 2);
						decoder((char*)ca_trainer_double_sentout);
						break;
						}
					case DOUBLE_TWO_AI:
						{
						// 1
						species = battle_field->battlers[3].species;
						name_by_species_to_buffer(species, 1);
						// 2
						species = battle_field->battlers[4].species;
						name_by_species_to_buffer(species, 2);
						decoder((char*)ca_trainer_doubleai_sentout);
						break;
						}
					case TRIPLE_TRAINER:
						{
						species = battle_field->battlers[3].species;
						name_by_species_to_buffer(species, 1);
						// 2
						species = battle_field->battlers[4].species;
						name_by_species_to_buffer(species, 2);
						// 2
						species = battle_field->battlers[5].species;
						name_by_species_to_buffer(species, 3);
						decoder((char*)ca_trainer_triple_sentout);
						break;
						}
					case HORDE_TRAINER:
						{
						species = battle_field->battlers[3].species;
						name_by_species_to_buffer(species, 1);
						decoder((char*)ca_trainer_horde_sentout);
						break;
						}
					default:
						break;
				};
				textbox_set_text(string_buffer, 1, 0, 1, 3, 1);
			}
			superstate.multi_purpose_state_tracker = 2;
			break;
			}
		case 5:
			// player send out.
			
		case 2:
		default:
			// idle state.
			break;
		};
	
	

}

void c1_battle_prep(void) {
	if (superstate.multi_purpose_state_tracker < 6) {
		void bs_exec(void);
		bs_exec();
		superstate.multi_purpose_state_tracker++;
	} else {
		//superstate.callback1 = (super_callback) 0x0000000;
		set_callback2(battle_prep_second_stage);
		set_callback1(battle_intro);
		superstate.multi_purpose_state_tracker = 0;
	}
}

void battle_prep() {
	u8 sub_8001960(void);
	void sub_80357C8(void);

		task_exec();
		objc_exec();
		obj_sync_something();

	switch(superstate.multi_purpose_state_tracker) {
		case 0:	
			{
			if (!sub_8001960()) {
				gpu_sync_bg_show(0);
				gpu_sync_bg_show(1);
				gpu_sync_bg_show(2);
				gpu_sync_bg_show(3);
				sub_80357C8();
				superstate.multi_purpose_state_tracker++;
				*battle_type_flags = 4;

				void b_setup_bx(void);
				b_setup_bx();
			}
			break;
			}
		case 1:
			{
			superstate.multi_purpose_state_tracker = 0;
			set_callback1(c1_battle_prep);
			set_callback2(battle_prep_second_stage);
			break;
			}
		default:
			break;
		};
	return;
}




struct battle_field* battle_init(struct battle_config *b_config, struct battle_field* battle_field) {
	battle_field->b_config = b_config;
	battle_field->battle_type = b_config->type;
	// set up battlers by battle type
	switch (b_config->type) {
		case SINGLE_WILD:
		case SINGLE_TRAINER:
			battle_field->active_battler_count_max = 2;
			battler_init(&battle_field->battlers[0], &pokemon_bank[6], 6); // player's first pkmn
			battler_init(&battle_field->battlers[3], &pokemon_bank[0], 0); // opponent's first pkmn
			battle_field->trainer_flag[0] = b_config->opp_id[0];
			break;
		case DOUBLE_WILD:
		case DOUBLE_TRAINER:
			battle_field->active_battler_count_max = 4;
			battler_init(&battle_field->battlers[0], &pokemon_bank[6], 6); // player's 1st pkmn
			battler_init(&battle_field->battlers[1], &pokemon_bank[7], 7); // player's 2nd pkmn
			battler_init(&battle_field->battlers[3], &pokemon_bank[0], 0); // opponent's 1st pkmn
			battler_init(&battle_field->battlers[4], &pokemon_bank[1], 1); // opponent's 2nd pkmn
			battle_field->trainer_flag[0] = b_config->opp_id[0];
			battle_field->trainer_flag[1] = b_config->opp_id[1];
			break;
		case DOUBLE_TWO_AI:
			battle_field->active_battler_count_max = 4;
			battler_init(&battle_field->battlers[0], &pokemon_bank[6], 6); // player's 1st pkmn
			battler_init(&battle_field->battlers[1], &pokemon_bank[9], 9); // ally's 1st pkmn
			battler_init(&battle_field->battlers[3], &pokemon_bank[0], 0); // opponent1's 1st pkmn
			battler_init(&battle_field->battlers[4], &pokemon_bank[3], 3); // opponent2's 2nd pkmn
			battle_field->trainer_flag[0] = b_config->opp_id[0];
			battle_field->trainer_flag[1] = b_config->opp_id[1];
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
			battle_field->trainer_flag[0] = b_config->opp_id[0];
			battle_field->trainer_flag[1] = b_config->opp_id[1];
			battle_field->trainer_flag[2] = b_config->opp_id[2];
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
			battle_field->trainer_flag[0] = b_config->opp_id[0];
			battle_field->trainer_flag[1] = b_config->opp_id[1];
			battle_field->trainer_flag[2] = b_config->opp_id[2];
			battle_field->trainer_flag[3] = b_config->opp_id[3];
			battle_field->trainer_flag[4] = b_config->opp_id[4];
			break;
		default:
			break;
	};
	return battle_field;
}




void battle_malloc_resources() {

	 battle_mallocd_resources.b_config = malloc_and_clear(sizeof(struct battle_config));
	 battle_mallocd_resources.battle_field = malloc_and_clear(sizeof(struct battle_field));
	 // more to come, maybe.
}



void store_byte_skipping(u16 *ptr, u16 pattern, u16 size) {
	u8 i;
	for (i = 0; i < size; i++) {
		*ptr = pattern;
		ptr += 1;
	}	
};

void battle_slidein_bg(struct battle_config *b_config) {
	hblank_handler_set(0);
	vblank_handler_set(0);
	u8 VRAM = 0x06;
	temp_vars.var_8000 = 0;
	cpu_set(&temp_vars.var_8000, (void *)(VRAM << 24), unk_05006000);
	lcd_io_set(0x4C, 0);
	lcd_io_set(0x40, 240);
	lcd_io_set(0x44, c_x5051);
	lcd_io_set(0x48, 0);
	lcd_io_set(0x4A, 0);
	vblank_cb_battle_WIN0H = 240;
	vblank_cb_battle_WIN0V = c_x5051;
	dp12_reset_something();
	store_byte_skipping(dp12_2038700, 0xF0, 0x4F);
	store_byte_skipping(dp12_big_2038700, 0xF0, 0x4F);
	store_byte_skipping(dp12_big1_2038700, 0xFF10, 0x4F);
	store_byte_skipping(dp12_big2_2038700, 0xFF10, 0x4F);
	
	sub_8087EE4(c_x0400001C, c_xA2600001, 0x1);
	pal_fade_control_and_dead_struct_reset();
	
	vblank_cb_battle_BG0HOFS = 0;
	vblank_cb_battle_BG0VOFS = 0;
	vblank_cb_battle_BG1HOFS = 0;
	vblank_cb_battle_BG1VOFS = 0;
	vblank_cb_battle_BG2HOFS = 0;
	vblank_cb_battle_BG2VOFS = 0;
	vblank_cb_battle_BG3HOFS = 0;
	vblank_cb_battle_BG3VOFS = 0;

	
	*idx_for_battle_screen_elements_by_env = b_config->env_by_map;
	sub_0800F34C();
	sub_0800F420();
	obj_and_aux_reset_all();
	tasks_init();
	
	/*
	
	0 - grass
	*/
	load_battle_screen_elements2(b_config->env_by_map);
	gpu_pal_allocator_reset();

	*gpu_pal_tag_search_lower_boundary = 4;
	vblank_handler_set(vblank_cb_battle);
	
	superstate.multi_purpose_state_tracker = 0;
	set_callback2(battle_prep);
	

	

}


void instanciate_opponent_party_single_double(u16 TID) {
	// calculate opponent party size
	u8 poke_count = ai_trainer[TID].poke_count;
	if (ai_trainer[TID].custom_attacks) {
		// custom attacks ver
		struct battle_template_custom *btemp = (struct battle_template_custom *)ai_trainer[TID].template_ptr;
		u8 i;
		for (i = 0; i < poke_count; i++) {
			create_pokemon((void *)&pokemon_bank[i], btemp->species, btemp->level, 0, 0, 0, 0, 0);

			set_attr(&pokemon_bank[i], ATTR_ATTACK_1, &(btemp->move[0]));
			set_attr(&pokemon_bank[i], ATTR_ATTACK_2, &(btemp->move[1]));
			set_attr(&pokemon_bank[i], ATTR_ATTACK_3, &(btemp->move[2]));
			set_attr(&pokemon_bank[i], ATTR_ATTACK_4, &(btemp->move[3]));
			btemp += sizeof(btemp);
		}
	} else {
		// normal ver 
		struct battle_template *btemp = (struct battle_template *)ai_trainer[TID].template_ptr;
		u8 i;
		for (i = 0; i < poke_count; i++) {
			create_pokemon((void *)&pokemon_bank[i], btemp->species, btemp->level, 0, 0, 0, 0, 0);
			btemp += sizeof(btemp);
		}
	}
}

void z_battle_setup () {
	// allocate resources @start
	battle_malloc_resources();

	
	struct battle_config *b_config = battle_mallocd_resources.b_config;
	struct battle_field *battle_field = battle_mallocd_resources.battle_field;
	
	// set up battle properties
	b_config->type = SINGLE_TRAINER;
	b_config->callback_return = c2_exit_to_overworld_1_continue_scripts_and_music;
	b_config->whiteout_switch = true; // enable whiteout 
	b_config->money_switch = true; // enable money gain
	b_config->exp_switch = true; // enable exp gain
	b_config->ai_difficulty = 0xFF; // hard
	b_config->env_by_map = 0; // grass
	b_config->opp_id[0] = 0x255;
	b_config->opp_id[1] = 0x256;
	b_config->opp_id[2] = 0x257;
	b_config->opp_id[3] = 0x258;
	b_config->opp_id[4] = 0x259;
	b_config->opponent_count = 0x5; // one opp 
	b_config->ally_backsprite = 0x2;

	instanciate_opponent_party_single_double(b_config->opp_id[0]);
	// battle_field fill battlers
	battle_field = battle_init(b_config, battle_field);
	battle_slidein_bg(b_config);
};

