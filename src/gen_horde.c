#include "types.h"
#include "ROM_tables.h"

u8 wild_pokemon_rand_grass();
u8 wild_pokemon_rand_water();
u8 wild_pokemon_gen_level(void *level_limits);
u8 repel_check_lead_level(u8 level);
void pokemon_purge_opponent();
u16 rand();
u32 divmod_remainder(u32 dividend, u32 divisor);
void pokemon_make_2(void *offset, u16 species, u16 level, u8 iv, u8 pid_indicator, u32 pid, u8 tid_sid_xor_indicator, u32 custom_tid_sid);
u32 gen_unown_pid(u8 letter);
u8 is_it_battle_time_1(u32 block);
u8 roll_sixty_percent();
void repel_something(u8 encounter_rate);
u8 roamer_roll();
void check_if_in_safari();
u16 get_wild_data_index();
u8 blockinfo_get_field(u32 block, u8 mask);
u8 walkrun_bitfield_and_r0(u32 bitfield);
u8 is_tile_XX_no_water_battles(u8 blockinfo);
u8 wild_roll_prob(u8 encounter_rate, u8 something);
void set_battle_style_roam();
u8 is_it_battle_time_1(u32 block);



void build_wild(u16 species, u8 level, u8 wild_index, void *enemy_offset) {
	if (species != 0xC9) {	//unown
	pokemon_make_2(enemy_offset, species, level, 0x20, 0x0, 0x0, 0x0, 0x0);
	return;
	}
    u8 map = saveblock1.sav1->player_map;
	u8 *unown_by_map = (u8 *)0x083CA71C;    //unown letters tbl
	u32 unown_pid = gen_unown_pid(*unown_by_map + ((map - 0x1B) * 12) + wild_index);
	pokemon_make_2(enemy_offset, 0xC9, level, 0x20, 0x1, unown_pid, 0x0, 0x0);
	return;
}



u8 map_index_grass_to_wild(u8 map, u8 quantity, u8 repel_possible) {
	u8 roll = wild_pokemon_rand_grass();
    u8 level = wild_pokemon_gen_level(&wild_pokemon_data[map].grass->grass_list->wild_pkmn[roll].min_level);
	if ((repel_possible == 0x1) && ((repel_check_lead_level(level)) == 0x0)) {
			return 0x0;
	}
    u16 species = wild_pokemon_data[map].grass->grass_list->wild_pkmn[roll].species;
	void pokemon_purge_opponent();
	void *party_opponent = (void *)0x0202402C;
	build_wild(species, level, roll, party_opponent);
	if (quantity <= 0x1) {
		return 0x1;
	}
	if (quantity == 0x2) {
		return 0x1;	//doubles, later
	}
	u8 loop;
    for (loop = 0x1; loop < quantity; loop ++) {
        do {
            roll = wild_pokemon_rand_grass();
        } while (species != wild_pokemon_data[map].grass->grass_list->wild_pkmn[roll].species);
        //matching species found
        level = wild_pokemon_gen_level(&wild_pokemon_data[map].grass->grass_list->wild_pkmn[roll].min_level);
        build_wild(species, level, roll, party_opponent + (loop  * 0x64));
    }
    return 0x1;
}



u8 map_index_water_to_wild(u8 map, u8 quantity, u8 repel_possible) {
    u8 roll = wild_pokemon_rand_water();
    u8 level = wild_pokemon_gen_level(&wild_pokemon_data[map].water->water_list->wild_pkmn[roll].min_level);
    if ((repel_possible == 0x1) && ((repel_check_lead_level(level)) == 0x0)) {
        return 0x0;
    }
    u16 species = wild_pokemon_data[map].water->water_list->wild_pkmn[roll].species;
    void pokemon_purge_opponent();
    void *party_opponent = (void *)0x0202402C;
    build_wild(species, level, roll, party_opponent);
    if (quantity <= 0x1) {
        return 0x1;
    }
    if (quantity == 0x2) {
        return 0x1;	//doubles, later
    }
    u8 loop;
    for (loop = 1; loop < quantity; loop ++) {
        do {
            roll = wild_pokemon_rand_water();
        } while (species != wild_pokemon_data[map].water->water_list->wild_pkmn[roll].species);
        u8 level = wild_pokemon_gen_level(&wild_pokemon_data[map].water->water_list->wild_pkmn[roll].min_level);
        build_wild(species, level, roll, party_opponent + (loop * 0x64));
    }
    return 0x1;
}



u8 is_it_battle_time_2(u32 block, u16 tile, u8 quantity) {
	u8 *byte_something = (u8 *)0x020386DC;
	if (*byte_something == 0x1) {
		return 0x0;
	}
	u16 wild_data_index = get_wild_data_index();
	if (wild_data_index == 0xFFFF)	 {
		return 0x0;
	}
	u8 block_field = blockinfo_get_field(block, 0x4);
	if (block_field == 0x1) {	//IF GRASS
		if ((wild_pokemon_data[wild_data_index].grass == 0x0) || ((blockinfo_get_field(block, 0x0) != tile) && (roll_sixty_percent() == 0x0))) {
            return 0x0;
		}
		u8 encounter_rate = wild_pokemon_data[wild_data_index].grass->encounter_chance;
		if (wild_roll_prob(encounter_rate, 0x0) != 0x1) {
			repel_something(encounter_rate);
				return 0x0;
		}
		if (roamer_roll() == 0x1) {
            if (repel_check_lead_level(saveblock1.sav1->roamer_level) == 0x0) {
                return 0x0;
            }
            set_battle_style_roam();
            return 0x1;
        }
		if (map_index_grass_to_wild(wild_data_index, quantity, 0x1) == 0x0) {
            repel_something(encounter_rate);
            return 0x0;
		}
		check_if_in_safari();
		return 0x1;
	}
//WATER
	if (block_field != 0x2) {
		if ((walkrun_bitfield_and_r0(0x8) == 0x0) || (is_tile_XX_no_water_battles(blockinfo_get_field(block, 0x0)) != 0x1)) {
			return 0x0;
		}
    }
    if (wild_pokemon_data[wild_data_index].water == 0x0) {
        return 0x0;
    }
    if ((blockinfo_get_field(block, 0x0) != tile) && (roll_sixty_percent() == 0x0)) {
        return 0x0;
    }
    u8 encounter_rate = wild_pokemon_data[wild_data_index].water->encounter_chance;
	if (encounter_rate != 0x1) {
		repel_something(encounter_rate);
        return 0x0;
		}
    if (roamer_roll() == 0x1) {
        if (repel_check_lead_level(saveblock1.sav1->roamer_level) == 0x0) {
            return 0x0;
        }
        set_battle_style_roam();
        return 0x1;
    }
    if (map_index_water_to_wild(wild_data_index, quantity, 0x1) == 0x0) {
        repel_something(encounter_rate);
        return 0x0;
    }
    check_if_in_safari();
    return 0x1;
}



u8 is_it_battle_time_3(u32 block, u8 quantity) {
    u16 x;
    u16 *short_ptr = (u16 *)0x020386D0;
    if (is_it_battle_time_1(block) == 0x0) {
        x = blockinfo_get_field(block, 0x0);
        *(short_ptr + 2) = x;   //+4
        return 0x0;
    }
    x = *(short_ptr + 2);       //+4
    if (is_it_battle_time_2(block, x, quantity) != 0x1) {
        x = blockinfo_get_field(block, 0x0);
        *(short_ptr + 2) = x;   //+4
        return 0x0;
    }
    x = 0x0;
    *(short_ptr + 3) = x;
    u8 *byte_ptr = (u8 *)0x020386D0;
    *(byte_ptr + 8) = x;
    x = blockinfo_get_field(block, x);
    *(short_ptr + 2) = x;
    return 0x1;
}




