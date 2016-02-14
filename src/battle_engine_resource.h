#ifndef BATTLE_LOCAL_RESOURCES
#define BATTLE_LOCAL_RESOURCES

/* Battle setup structs */

#include "types.h"
#include "ROM_tables.h"

enum battle_result {
	RESULT_WIN,
	RESULT_LOSS,
	RESULT_TIE, // Low priority. Case of double wipeout.
	RESULT_RUN,
	RESULT_CAPTURED,
	RESULT_FORFEIT, // Low priority. Forfeit trainer battles
	RESULT_ON_GOING,
	RESULT_TO_CONTINUE, // exit to OW, don't free structs
};

struct flags {
	u8 effectiveness; // 1 Not effective, 2 super, 0 normal
	u8 crit_flag : 1;
	u8 health_was_full : 1;
	u8 abilities_disabled : 1;
	u8 items_disabled : 1;
	u8 pursuit_flag : 1;
	u8 filler : 3;
};

struct battle_config {
	u8 type;
	//u8 purge_switch;
	super_callback callback_return;
	u8 whiteout_switch;
	u8 money_switch;
	u8 exp_switch;
	//u8 tut_mode;
	u8 ai_difficulty;
	u8 env_by_map;
	u16 opp_id[5];
	//u8 frontier_mode;
	u8 opponent_count;
	//u8 player_control_ally_count;
	//u8 ai_control_ally_count;
	u8 ally_backsprite;
	struct flags *flags;
	u16 opponent_id;
};

struct battler {
	u8 obj_id;
	u8 ability;
	u16 species; // for my laziness
	u16 weight;
	u8 level;
	u8 types[3]; // 3 types, yay gen 6!
	u16 item;
	u16 current_hp;
	u16 total_hp;
	u16 attack;
	u16 defense;
	u16 speed;
	u16 sp_attack;
	u16 sp_defense;
	u32 ailment;
	u16 moves_used[4];
	u16 move_used_last;
	u8 last_move_target;
	u16 damage_taken;
	u8 damage_taken_type;
	u8 damaged_by_whom; // u8 damager ID
	u16 damage_done; // turn basis
	u16 queued_move;
	u8 multi_hit; // how many hits so far
	u8 stock_pile;
	u8 pokemon_bank_id;
	u8 turns_in_battle;
	
	
	u8 metronome;
	u8 toxic; // turns count
	u8 leech_seed; //pokemon in which slot gets the HP
	u8 disable; // turns remaining
	u8 taunt; // turns remaining
	u8 torment; // turns remaining
	u8 encore; // turns remaining
	u8 bide; // turns remaining
	u8 mirrorcoat_counter; // activated. 1 = mirror, 2 = counter, 0 = inactive
	
	u8 furry_cutter; // times used
	u8 rollout_iceball; // times used
	
	u8 trap; // duration + dmg
	u16 trap_dmg;
	
	u32 attack_multiplier : 2;
	u32 attack_divider : 2;
	u32 defense_multiplier : 2;
	u32 defense_divider : 2;
	u32 special_attack_multiplier : 2;
	u32 special_attack_divider : 2;
	u32 special_defense_multiplier : 2;
	u32 special_defense_divider : 2;
	u32 speed_multiplier : 2;
	u32 speed_divider : 2;
	u32 evasion_multiplier : 2;
	u32 evasion_divider : 2;
	u32 accuracy_multiplier : 2;
	u32 accuracy_divider : 2;
	u32 crit_multiplier : 2;
	u32 crit_divider : 2;
	
	// statuses and things the user can be under
	u16 substitute : 1;
	u16 perish_song : 1;
	u16 electrify : 1;
	u16 bounce : 1;
	u16 dig : 1;
	u16 dive : 1;
	u16 fly : 1;
	u16 paddin_bit : 1;
	u16 shadow_force : 1;
	u16 sky_drop : 1;
	u16 lock_on_mind_reader : 1;
	u16 wide_gaurd : 1;
	u16 protect : 1;
	u16 influation : 1;
	u16 floored : 1; // gravity, magnet rise, ect.
	u16 battle_side : 1; // 0 = player side, 1 = opponent side
	
	u16 minimize : 1;
	u16 defense_curl : 1;
	u16 confused : 1;
	u16 odor_sleuth : 1;
	u16 helping_hand : 1;
	u16 charge : 1;
	u16 turn_skip : 1;
	u16 focus_punch : 1;
	u16 outrage : 1;
	u16 me_first : 1;
	u16 attacked : 1;
	u16 gender : 1;
	u16 transformed : 1;
	u16 gastro_acid : 1;
	u16 embargo : 1;
	u16 free_space : 2;
};

struct field_modifiers {
	// holds layers count/activated count per side
	u8 spikes[2];
	u8 toxic_spikes[2];
	u8 stealth_rocks[2];
	u8 sticky_web[2];
	u8 fairy_lock[2];
	u8 magnetic_flux;
	u8 fainted[2]; // something fainted on side
	
	
	// turns count or team-wide status inducers
	u8 trick_room[2]; // first byte is type of room, wonder magic or trick
	u8 ion_deluge;
	u8 gaurd[4]; // [0] used player side, [1] type, ...
	
	// type and durations
	u8 weather[2];
	
	// durations
	u8 mud_sport;
	u8 water_sport;
	
	/* 
	these arrays are of style:
	
	tailwind[0] = 1 if player's side has wind
	tailwind[1] = Duration counter for tailwind player
	tailwind[2] = 1 if opponent's side has wind
	tailwind[3] = Duration counter for tailwind	opponent
	*/	
	u8 tailwind[4]; 
	u8 reflect[4];
	u8 lightscreen[4];
	u8 safeguard[4];
	u8 terrain[4];
	u8 pledge[4];
	u8 wish[12];
	
	/* 
	move[0] = 1 if pokemon at slot was choosen as target to recieve
	move[1] = duration till attack	
	*/
	u8 future_sight[12];
	u8 doomdesire[12];
	u8 echoed_voice[2]; // times used successively
	u8 round[2]; // allies used round.
};

struct battle_field {
	u16 trainer_flag[5];
	u8 battle_type;
	u8 turn_counter;
	struct battle_config *b_config;
	struct field_modifiers modifiers;
	struct battler battlers[6]; // potential for 6 on field
	struct battle_flags *b_flags;
	u16 moves_used[6]; // moves used this turn 
	u8 ally[6];
	u8 active_battler_count;
	u8 active_battler_count_max;
	enum battle_result battle_result;
	u16 multi_use;
};


struct battle_mallocd_resources {
	struct battle_config *b_config;
	struct battle_field *battle_field;
	struct objtemplate *objtemp[20]; // maybe too much
	struct resource *resources[20];
	u8 ids_in_use[20];
	u8 free_use_flags[20];
	
};

extern struct battle_mallocd_resources battle_mallocd_resources;
extern struct battle_field battle_field;
extern enum battle_result battle_result;

#endif /* BATTLE_LOCAL_RESOURCES */