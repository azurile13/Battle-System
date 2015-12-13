#include "battle_engine_resource.h"
#include "ROM_tables.h"
#include "multipurpose_resources.c"
#include "types.h"
#include "defines.h"

#define SINGLE 1
#define HORDE 5


u8 get_ability(struct battle_field *battle_field, struct battler *attacker, struct flags *flags) {
	if (flags->abilities_disabled) {
		return false;
	}
	if (attacker->gastro_acid) {
		switch (attacker->ability) {
			case ABILITY_MULTITYPE:
			case ABILITY_STANCE_CHANGE:
			case ABILITY_MAGIC_BOUNCE:
				return attacker->ability;
				break;
			default:
				return false;
				break;
		};
	}
	return attacker->ability;
}


u8 ability_present(struct battle_field *battle_field, u8 ability, struct flags *flags) {
	if (flags->items_disabled) {
		return false;
	}
	u16 i;
	for (i = 0; i < 6; i++) {
		if (battle_field->battlers[i].ability == ability) {
			if (get_ability(battle_field, &(battle_field->battlers[i]), flags) > 0) {
				return true;
			}
		}
	}
	return false;
}

u8 ability_present_ally(struct battle_field *battle_field, u8 ability, struct flags *flags) {
	if (flags->items_disabled) {
		return false;
	}
	u16 i;
	for (i = 0; i < 6; i++) {
		if (battle_field->battlers[i].ability == ability) {
			return battle_field->ally[i] + 1;
		}
	}
	return false;
}

u8 apply_mold_breaker(struct battle_field *battle_field, struct battler *defender, struct battler *attacker, struct flags *flags) {
	u8 ability = get_ability(battle_field, attacker, flags);

	if (flags->abilities_disabled) {
		return false;
	}

	if ((attacker->gastro_acid) && (!defender->gastro_acid)) {
		return defender->ability;
	}

	if ((ability == ABILITY_MOLD_BREAKER) || (ability == ABILITY_TERAVOLT) ||
	(ability == ABILITY_TURBOBLAZE)) {
			switch (defender->ability) {
				case ABILITY_BATTLE_ARMOR:
				case ABILITY_CLEAR_BODY:
				case ABILITY_DAMP:
				case ABILITY_DRY_SKIN:
				case ABILITY_FILTER:
				case ABILITY_FLASH_FIRE:
				case ABILITY_FLOWER_GIFT:
				case ABILITY_HEATPROOF:
				case ABILITY_HYPER_CUTTER:
				case ABILITY_IMMUNITY:
				case ABILITY_INNER_FOCUS:
				case ABILITY_INSOMNIA:
				case ABILITY_KEEN_EYE:
				case ABILITY_LEAF_GUARD:
				case ABILITY_LEVITATE:
				case ABILITY_LIGHTNING_ROD:
				case ABILITY_LIMBER:
				case ABILITY_MAGMA_ARMOR:
				case ABILITY_MARVEL_SCALE:
				case ABILITY_MOTOR_DRIVE:
				case ABILITY_OBLIVIOUS:
				case ABILITY_OWN_TEMPO:
				case ABILITY_SAND_VEIL:
				case ABILITY_SHELL_ARMOR:
				case ABILITY_SHIELD_DUST:
				case ABILITY_SIMPLE:
				case ABILITY_SNOW_CLOAK:
				case ABILITY_SOLID_ROCK:
				case ABILITY_SOUNDPROOF:
				case ABILITY_STICKY_HOLD:
				case ABILITY_STORM_DRAIN:
				case ABILITY_STURDY:
				case ABILITY_SUCTION_CUPS:
				case ABILITY_TANGLED_FEET:
				case ABILITY_THICK_FAT:
				case ABILITY_UNAWARE:
				case ABILITY_VITAL_SPIRIT:
				case ABILITY_VOLT_ABSORB:
				case ABILITY_WATER_ABSORB:
				case ABILITY_WATER_VEIL:
				case ABILITY_WHITE_SMOKE:
				case ABILITY_WONDER_GUARD:
				case ABILITY_BIG_PECKS:
				case ABILITY_CONTRARY:
				case ABILITY_FRIEND_GUARD:
				case ABILITY_HEAVY_METAL:
				case ABILITY_LIGHT_METAL:
				case ABILITY_MAGIC_BOUNCE:
				case ABILITY_MULTISCALE:
				case ABILITY_SAP_SIPPER:
				case ABILITY_TELEPATHY:
				case ABILITY_WONDER_SKIN:
				case ABILITY_AROMA_VEIL:
				case ABILITY_BULLETPROOF:
				case ABILITY_FLOWER_VEIL:
				case ABILITY_FUR_COAT:
				case ABILITY_OVERCOAT:
				case ABILITY_SWEET_VEIL:
					return false;
					break;
				default:
					break;
			};
		}
	return defender->ability;

}

u8 is_type(struct battler *attacker, u8 type) {
	u16 i;
	for (i = 0; i < 3; i++) {
		if (type == attacker->types[i]){
			return true; // is type
		}
	}
	return false; // isn't type
}

u8 holding_mega_stone(struct battler *attacker) {
	u16 i;
	for (i = 0; i < sizeof(mega_items_t); i++) {
		if (attacker->item == mega_items_t[i].item_id) {
			return attacker->item;
		}
	}
	return false;
}

u16 get_item(struct battle_field *battle_field, struct battler *attacker, struct flags *flags) {
	// mega stones are unaffected by everything ever
	u16 item = holding_mega_stone(attacker);
	if (item) {
		return item;
	}

	if (attacker->embargo || flags->items_disabled) {
		return item;
	}

	if (ability_present(battle_field, ABILITY_UNNERVE, flags)) {
		// can't use berry
		u16 i;
		for (i = 0; i < sizeof(berries_t); i++) {
			if (attacker->item == berries_t[i].item_id) {
				return ITEM_NONE;
			}
		}
	}

	if (battle_field->modifiers.trick_room[0]) {
		return ITEM_NONE;
	}

	// items in battle not effected by klutz
	switch (attacker->item) {
		case ITEM_AMULET_COIN:
		case ITEM_LUCKY_EGG:
		case ITEM_POWER_WEIGHT:
		case ITEM_IRON_BALL:
			return attacker->item;
		default:
			break;
	};

	if (get_ability(battle_field, attacker, flags) != ABILITY_KLUTZ) {
		return attacker->item;
	}
	return ITEM_NONE;
}

u16 get_def(struct battler *attacker, u8 is_special) {
	u16 type;
	if (is_special) {
		type = attacker->sp_defense;
	} else {
		type = attacker->defense;
	}
	return type;
}

u16 get_species(struct battler *attacker) {
	u16 species = attacker->species;
	return species;
}

u8 get_weather(struct battle_field *battle_field) {
	u8 weather = battle_field->modifiers.weather[0];
	return weather;
}

u8 apply_dmg_mod(u16 current_dmg, u8 percentage, u8 negativity) {
	// negativity = 0 means reduce, else increase by percent.
	if (!percentage) {
		return current_dmg;
	}
	u16 temp = (current_dmg * percentage) / 100;
	if (negativity) {
		return temp + current_dmg;
	} else {
		return temp;
	}
}

u8 not_immune(struct battler *defender, u8 type) {
	/*

	HUUUGGEEE overhaul needed.

	*/
	// return 0 if immune to attack -- if not return type effectiveness
	u8 effectiveness = 0;
	u8 immunity_flag_ghost = 0;
	type = (type_total * type) + defender->types[1];

	if (type == TYPE_GHOST && defender->odor_sleuth) {
		immunity_flag_ghost = 1;
	}
	u8 temp = type_chart[(type_total * type) + defender->types[1]].something;

	u16 i;
	for (i = 0; i < 3; i++) {
		switch (temp) {
			case 1:
				temp = 50;
				break;
			case 2:
				temp = 100;
				break;
			case 3:
				temp = 200;
				break;
			default:
				if (immunity_flag_ghost > 0) {
					temp = 100;
				} else {
					temp = 0;
				}
				break;
		};
		effectiveness *= temp;
	}
	return effectiveness / 10000; // type in percent damage taken
}

u8 get_item_modifier(struct battle_field *battle_field, struct battler *attacker, struct move_table *attack, struct flags *flags){
	u16 item_boost = 0;
	u8 physical = attack->is_special;
	u8 atk_type = attack->type;
	u16 item = get_item(battle_field, attacker, flags);
	u16 species = get_species(attacker);

	switch (item) {
		case ITEM_LUSTROUS_ORB:
			if ((atk_type == TYPE_WATER) || (atk_type == TYPE_DRAGON)){
				if (species == POKE_PALKIA) {
					item_boost = 20;
				}
			}
			break;
		case ITEM_ADAMANT_ORB:
			if ((atk_type == TYPE_STEEL) || (atk_type == TYPE_DRAGON)){
				if (species == POKE_DIALGA) {
					item_boost = 20;
				}
			}
			break;
		case ITEM_GRISEOUS_ORB:
			if ((atk_type == TYPE_GHOST) || (atk_type == TYPE_DRAGON)){
				if ((species == POKE_GIRATINA) || (species == POKE_GIRATINA_O)) {
					item_boost = 20;
				}
			}
			break;
		case ITEM_LIFE_ORB:
			item_boost = 30;
			break;
		case ITEM_METRONOME_item:
			item_boost = (attacker->metronome) * 20;
			break;
		case ITEM_MUSCLE_BAND:
			if (!physical) {
				item_boost = 10;
			}
			break;
		case ITEM_WISE_GLASSES:
			if (physical) {
				item_boost = 10;
			}
			break;
		case ITEM_DRAGON_GEM:
			if (atk_type == TYPE_DRAGON) {
				item_boost = 30;
			}
			break;
		case ITEM_DRACO_PLATE:
		case ITEM_DRAGON_FANG:
			if (atk_type == TYPE_DRAGON) {
				item_boost = 20;
			}
			break;
		case ITEM_DARK_GEM:
			if (atk_type == TYPE_DARK) {
				item_boost = 30;
			}
			break;
		case ITEM_DREAD_PLATE:
		case ITEM_BLACK_GLASSES:
			if (atk_type == TYPE_DARK) {
				item_boost = 20;
			}
			break;
		case ITEM_GROUND_GEM:
			if (atk_type == TYPE_GROUND) {
				item_boost = 30;
			}
			break;
		case ITEM_EARTH_PLATE:
		case ITEM_SOFT_SAND:
			if (atk_type == TYPE_GROUND) {
				item_boost = 20;
			}
			break;
		case ITEM_FIGHTING_GEM:
			if (atk_type == TYPE_FIGHTING) {
				item_boost = 30;
			}
			break;
		case ITEM_FIST_PLATE:
		case ITEM_BLACK_BELT:
			if (atk_type == TYPE_FIGHTING) {
				item_boost = 20;
			}
			break;
		case ITEM_FIRE_GEM:
			if (atk_type == TYPE_FIRE) {
				item_boost = 30;
			}
			break;
		case ITEM_FLAME_PLATE:
		case ITEM_CHARCOAL:
			if (atk_type == TYPE_FIRE) {
				item_boost = 20;
			}
			break;
		case ITEM_ICE_GEM:
			if (atk_type == TYPE_ICE) {
				item_boost = 30;
			}
			break;
		case ITEM_ICICLE_PLATE:
		case ITEM_NEVER_MELT_ICE:
			if (atk_type == TYPE_ICE) {
				item_boost = 20;
			}
			break;
		case ITEM_BUG_GEM:
			if (atk_type == TYPE_BUG) {
				item_boost = 30;
			}
			break;
		case ITEM_INSECT_PLATE:
		case ITEM_SILVER_POWDER:
			if (atk_type == TYPE_BUG) {
				item_boost = 20;
			}
			break;
		case ITEM_STEEL_GEM:
			if (atk_type == TYPE_STEEL) {
				item_boost = 30;
			}
			break;
		case ITEM_IRON_PLATE:
		case ITEM_METAL_COAT:
			if (atk_type == TYPE_STEEL) {
				item_boost = 20;
			}
			break;
		case ITEM_GRASS_GEM:
			if (atk_type == TYPE_GRASS) {
				item_boost = 30;
			}
			break;
		case ITEM_MEADOW_PLATE:
		case ITEM_MIRACLE_SEED:
		case ITEM_ROSE_INCENSE:
			if (atk_type == TYPE_GRASS) {
				item_boost = 20;
			}
			break;
		case ITEM_PSYCHIC_GEM:
			if (atk_type == TYPE_PSYCHIC) {
				item_boost = 30;
			}
			break;
		case ITEM_MIND_PLATE:
		case ITEM_TWISTED_SPOON:
		case ITEM_ODD_INCENSE:
			if (atk_type == TYPE_PSYCHIC) {
				item_boost = 20;
			}
			break;
		case ITEM_FAIRY_GEM:
			if (atk_type == TYPE_FAIRY) {
				item_boost = 30;
			}
			break;
		case ITEM_PIXIE_PLATE:
			if (atk_type == TYPE_FAIRY) {
				item_boost = 20;
			}
			break;
		case ITEM_FLYING_GEM:
			if (atk_type == TYPE_FLYING) {
				item_boost = 30;
			}
			break;
		case ITEM_SKY_PLATE:
		case ITEM_SHARP_BEAK:
			if (atk_type == TYPE_FLYING) {
				item_boost = 20;
			}
			break;
		case ITEM_WATER_GEM:
			if (atk_type == TYPE_WATER) {
				item_boost = 30;
			}
			break;
		case ITEM_SPLASH_PLATE:
		case ITEM_MYSTIC_WATER:
		case ITEM_SEA_INCENSE:
		case ITEM_WAVE_INCENSE:
			if (atk_type == TYPE_WATER) {
				item_boost = 20;
			}
			break;
		case ITEM_GHOST_GEM:
			if (atk_type == TYPE_DRAGON) {
				item_boost = 30;
			}
			break;
		case ITEM_SPOOKY_PLATE:
		case ITEM_SPELL_TAG:
			if (atk_type == TYPE_GHOST) {
				item_boost = 20;
			}
			break;
		case ITEM_ROCK_GEM:
			if (atk_type == TYPE_ROCK) {
				item_boost = 30;
			}
			break;
		case ITEM_STONE_PLATE:
		case ITEM_HARD_STONE:
		case ITEM_ROCK_INCENSE:
			if (atk_type == TYPE_ROCK) {
				item_boost = 20;
			}
			break;
		case ITEM_POISON_GEM:
			if (atk_type == TYPE_POISON) {
				item_boost = 30;
			}
			break;
		case ITEM_TOXIC_PLATE:
		case ITEM_POISON_BARB:
			if (atk_type == TYPE_POISON) {
				item_boost = 20;
			}
			break;
		case ITEM_ELECTRIC_GEM:
			if (atk_type == TYPE_ELECTRIC) {
				item_boost = 30;
			}
			break;
		case ITEM_ZAP_PLATE:
		case ITEM_MAGNET:
			if (atk_type == TYPE_ELECTRIC) {
				item_boost = 20;
			}
			break;
		case ITEM_NORMAL_GEM:
			if (atk_type == TYPE_NORMAL) {
				item_boost = 30;
			}
			break;
		// case PINK_BOW: - removed after gen 2
		case ITEM_SILK_SCARF:
			if (atk_type == TYPE_NORMAL) {
				item_boost = 20;
			}
			break;
		default:
			item_boost = 0;
			break;
	};
	return item_boost;
}

u16 get_base_power(struct battle_field *battle_field, u8 attacker_id, u8 defender_id, struct move_table *attack, struct flags *flags) {
	struct battler *attacker = &battle_field->battlers[attacker_id];
	struct battler *defender = &battle_field->battlers[defender_id];
	u16 atk_base_power = 0;

	// some speed calculations for certain attacks
	u16 target_spd = defender->speed_multiplier;
	u16 user_spd = attacker->speed_multiplier;
	u16 t_base_spd = defender->speed;
	u16 u_base_spd = attacker->speed;
	// target's speed
	if (target_spd > 0) {
		target_spd *= t_base_spd;
	} else {
		target_spd = t_base_spd;
		if (defender->speed_divider) {
			target_spd =
			apply_dmg_mod((target_spd * 100), (25 * defender->speed_divider), 0);
		}
	}
	//user's speed
	if (user_spd > 0) {
		user_spd *= u_base_spd;
	} else {
		user_spd = u_base_spd;
		if (attacker->speed_divider) {
			user_spd =
			apply_dmg_mod((user_spd * 100), (25 * attacker->speed_divider), 0);
		}
	}

	switch (attack->move_id) {
		case MOVE_KNOCK_OFF:
			if (defender->item) {
				u8 i;
				for (i = 0; i < mega_items_table_size; i++) {
					if ((attacker->item == megas[i].item) &&
					(attacker->species == megas[i].species)) {
						atk_base_power = attack->base_power;
					}
				}
				if (atk_base_power) {
					atk_base_power = apply_dmg_mod(attack->base_power, 50, 1);
				}
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_FRUSTRATION:
		{
			atk_base_power = get_attr(&pokemon_bank[attacker_id], ATTR_HAPPINESS);
			atk_base_power = ((255 - atk_base_power) * 10) / 25;
			break;
		}
		case MOVE_PAYBACK:
			if (defender->damage_done > 0) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_RETURN:
		{
			atk_base_power = get_attr(&pokemon_bank[attacker->pokemon_bank_id], ATTR_HAPPINESS);
			atk_base_power = (atk_base_power * 10) / 25;
			break;
		}
		case MOVE_ELECTRO_BALL:
			if (user_spd < target_spd) {
				atk_base_power = 40;
			} else {
				// multiply by 1000 for divmod not to morph data too much
				atk_base_power = (user_spd * 1000) / target_spd;
				if (atk_base_power < 2000) {
					atk_base_power = 60;
				}
				if ((atk_base_power > 2000) && (atk_base_power < 3000)) {
					atk_base_power = 80;
				}

				if ((atk_base_power > 3000) && (atk_base_power < 4000)) {
					atk_base_power = 120;
				}

				if (atk_base_power > 4000) {
					atk_base_power = 150;
				}
			}
			break;
		case MOVE_AVALANCHE:
			if (defender->last_move_target == attacker_id) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_GYRO_BALL:
		{
			atk_base_power = (target_spd * 25) / user_spd;
			if (atk_base_power > 150) {
				atk_base_power = 150;
			}
			break;
		}
		case MOVE_ERUPTION:
		case MOVE_WATER_SPOUT:
		{
			atk_base_power = (attacker->current_hp) * 150;
			atk_base_power = atk_base_power / attacker->total_hp;
			break;
		}
		case MOVE_PUNISHMENT:
		{
			atk_base_power = defender->attack_multiplier + defender->defense_multiplier
			+ defender->special_attack_multiplier + defender->speed_multiplier +
			defender->special_defense_multiplier + defender->evasion_multiplier +
			defender->accuracy_multiplier + defender->crit_multiplier;
			atk_base_power = atk_base_power * 20 + 60;
			if (atk_base_power > 120) {
				atk_base_power = 120;
			}
			break;
		}
		case MOVE_FURY_CUTTER:
		{
			atk_base_power = attack->base_power;
			switch (attacker->furry_cutter) {
				case 1:
					atk_base_power *= 2;
					break;
				case 2:
					atk_base_power *= 4;
					break;
				case 3:
					atk_base_power *= 8;
					break;
				default:
					atk_base_power *= 1;
					break;
			};
		}
		case MOVE_GRASS_KNOT:
		case MOVE_LOW_KICK:
			if (defender->weight >= 200) {
				atk_base_power = 120;
				break;
			}
			if (defender->weight >= 100) {
				atk_base_power = 100;
				break;
			}
			if (defender->weight >= 50) {
				atk_base_power = 80;
				break;
			}
			if (defender->weight >= 25) {
				atk_base_power = 60;
				break;
			}
			if (defender->weight >= 10) {
				atk_base_power = 40;
				break;
			}
			atk_base_power = 20;
			break;
		case MOVE_ECHOED_VOICE:
		{
			atk_base_power = battle_field->modifiers.echoed_voice[attacker->battle_side];
			switch (atk_base_power) {
				case 0:
					atk_base_power = attack->base_power;
					break;
				case 1:
					atk_base_power = attack->base_power * 2;
					break;
				case 2:
					atk_base_power = attack->base_power * 3;
					break;
				default:
					atk_base_power = 200;
					break;
			};
		}
		case MOVE_HEX:
		{
			atk_base_power = defender->ailment;
			if (atk_base_power) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		}
		case MOVE_WRING_OUT:
		case MOVE_CRUSH_GRIP:
		{
			u8 c_hp = defender->current_hp * 100;
			u8 t_hp = defender->total_hp;
			atk_base_power = (c_hp / t_hp) * 120;
			atk_base_power = atk_base_power / 100;
			break;
		}
		case MOVE_ASSURANCE:
			if (defender->damage_taken) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_HEAVY_SLAM:
		case MOVE_HEAT_CRASH:
		{
			atk_base_power = (attacker->weight * 100) / defender->weight;
			if (atk_base_power >= 500) {
				atk_base_power = 120;
				break;
			}
			if (atk_base_power >= 400) {
				atk_base_power = 100;
			break;
			}
			if (atk_base_power >= 300) {
				atk_base_power = 80;
				break;
			}
			if (atk_base_power >= 200) {
				atk_base_power = 60;
				break;
			}
			atk_base_power = 40;
			break;
		}
		case MOVE_STORED_POWER:
			atk_base_power = attacker->attack_multiplier +
			attacker->defense_multiplier +
			attacker->special_attack_multiplier +
			attacker->speed_multiplier +
			attacker->special_defense_multiplier +
			attacker->evasion_multiplier +
			attacker->accuracy_multiplier;
			atk_base_power = 20 + (20 * atk_base_power);
			break;
		case MOVE_ACROBATICS:
			// can directly check since no ability/status grants items
			if (!attacker->item) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_FLAIL:
		case MOVE_REVERSAL:
		{
			atk_base_power = attacker->current_hp * 48;
			u16 temp = attacker->total_hp;
			atk_base_power = (atk_base_power * 1000) / temp;
			if (atk_base_power <= 1000) {
				atk_base_power = 200;
				break;
			}
			if ((atk_base_power > 1000) && (atk_base_power <= 4000)) {
				atk_base_power = 150;
				break;
			}
			if ((atk_base_power > 4000) && (atk_base_power <=  9000)) {
				atk_base_power = 100;
				break;
			}
			if ((atk_base_power > 9000) && (atk_base_power <= 16000)) {
				atk_base_power = 80;
				break;
			}
			if ((atk_base_power > 16000) && (atk_base_power <= 32000)) {
				atk_base_power = 40;
				break;
			}
			atk_base_power = 20;
			break;
		}
		case MOVE_TRUMP_CARD:
			// get PP of move
			{
			struct pokemon pkmn = pokemon_bank[attacker_id];
			if (get_attr(&pkmn, ATTR_ATTACK_1) == MOVE_TRUMP_CARD) {
				atk_base_power = get_attr(&pkmn, ATTR_PP_1);
			}
			if (get_attr(&pkmn, ATTR_ATTACK_2) == MOVE_TRUMP_CARD) {
				atk_base_power = get_attr(&pkmn, ATTR_PP_2);
			}
			if (get_attr(&pkmn, ATTR_ATTACK_3) == MOVE_TRUMP_CARD) {
				atk_base_power = get_attr(&pkmn, ATTR_PP_3);
			}
			if (get_attr(&pkmn, ATTR_ATTACK_4) == MOVE_TRUMP_CARD) {
				atk_base_power = get_attr(&pkmn, ATTR_PP_4);
			}
			switch (atk_base_power) {
				case 1:
				{
					atk_base_power = 200;
					break;
				}
				case 2:
				{
					atk_base_power = 80;
					break;
				}
				case 3:
				{
					atk_base_power = 60;
					break;
				}
				case 4:
				{
					atk_base_power = 50;
					break;
				}
				default:
				{
					atk_base_power = 40;
					break;
				}
			};
			break;
		}
		case MOVE_ROUND:
		{
			atk_base_power = battle_field->modifiers.round[attacker->battle_side];
			if (atk_base_power) {
				atk_base_power = attack->base_power;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		}
		case MOVE_TRIPLE_KICK:
			switch (attacker->multi_hit) {
				case 1:
					atk_base_power = attack->base_power;
					break;
				case 2:
					atk_base_power = attack->base_power * 2;
					break;
				case 3:
					atk_base_power = attack->base_power * 3;
					break;
			};
		case MOVE_WAKEUP_SLAP:
			if (defender->ailment == SLEEP) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_SMELLING_SALTS:
			if (defender->ailment == PARALYZE) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_WEATHER_BALL:
		{
			u8 weather = get_weather(battle_field);
			if (weather) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		}
		case MOVE_GUST:
		case MOVE_TWISTER:
		case MOVE_THUNDER:
			if (defender->bounce || defender->fly || defender->sky_drop) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_BEAT_UP:
		{
			u8 temp = attacker->multi_hit;
			atk_base_power = pokemon_bank[6 + temp].attack;
			atk_base_power = (atk_base_power / 10) + 5;
			break;
		}
		case MOVE_SPIT_UP:
		{
			atk_base_power = attacker->stock_pile * 100;
			break;
		}
		case MOVE_PURSUIT:
			if (flags->pursuit_flag) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_PRESENT:
		{
			atk_base_power = random(100);
			if (atk_base_power < 40) {
				atk_base_power = 40;
				break;
			}
			if (atk_base_power < 70) {
				atk_base_power = 80;
				break;
			}
			atk_base_power = 120;
			break;
		}
		case MOVE_NATURAL_GIFT:
		{
			u16 item = get_item(battle_field, attacker, flags);
			switch (item) {
				// to rework into table soon
				default:
					atk_base_power = 0;
					break;
			};
		}
		case MOVE_MAGNITUDE:
		{
			atk_base_power = random(100);
			if (atk_base_power < 5) {
				atk_base_power = 10;
				break;
			}
			if ((atk_base_power >= 5) && (atk_base_power < 15)) {
				atk_base_power = 10 + 20 * 1;
				break;
			}
			if ((atk_base_power >= 15) && (atk_base_power < 35)) {
				atk_base_power = 10 + 20 * 2;
				break;
			}
			if ((atk_base_power >= 35) && (atk_base_power < 65)) {
				atk_base_power = 10 + 20 * 3;
				break;
			}
			if ((atk_base_power >= 65) && (atk_base_power < 85)) {
				atk_base_power = 10 + 20 * 4;
				break;
			}
			if ((atk_base_power >= 85) && (atk_base_power < 95)) {
				atk_base_power = 10 + 20 * 5;
				break;
			}
			atk_base_power = 10 + 20 * 7;
			break;
		}
		case MOVE_ROLLOUT:
		case MOVE_ICE_BALL:
			atk_base_power = attacker->rollout_iceball + attacker->defense_curl;
			switch (atk_base_power) {
				case 1:
					atk_base_power = attack->base_power * 2;
					break;
				case 2:
					atk_base_power = attack->base_power * 4;
					break;
				case 3:
					atk_base_power = attack->base_power * 8;
					break;
				case 4:
					atk_base_power = attack->base_power * 16;
					break;
				case 5:
					atk_base_power = attack->base_power * 32;
					break;
				default:
					atk_base_power = attack->base_power * 64;
					break;
			};
			break;
		case MOVE_FLING:
			//todo : make table. god damn fling...
			atk_base_power = 40;
			break;
		case MOVE_GRASS_PLEDGE:
		case MOVE_FIRE_PLEDGE:
		case MOVE_WATER_PLEDGE:
		{
			u16 i;
			atk_base_power = attack->base_power;
			for (i = 0; i < 6; i++) {
				if ((!battle_field->ally[i]) && (i != attacker_id)) {
					if ((battle_field->battlers[i].queued_move == MOVE_GRASS_PLEDGE) ||
						(battle_field->battlers[i].queued_move == MOVE_FIRE_PLEDGE) ||
						(battle_field->battlers[i].queued_move == MOVE_WATER_PLEDGE)) {
						atk_base_power = 150;
						battle_field->battlers[i].turn_skip = 1;
						break;
					}
				}
			}
			break;
		}
		case MOVE_FACADE:
		{
			u8 ailment = attacker->ailment;
			if ((ailment == POISONNED) || (ailment == BURN) || (ailment == PARALYZE)) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		}
		case MOVE_BRINE:
			if ((defender->current_hp * 2) <= defender->total_hp) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_VENOSHOCK:
			if (defender->ailment == POISONNED) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_RETALIATE:
			if (battle_field->modifiers.fainted[attacker->battle_side]) {
				atk_base_power = attack->base_power * 2;
			} else {
				atk_base_power = attack->base_power;
			}
			break;
		case MOVE_FUSION_BOLT:
		{
			atk_base_power = 0;
			u16 i, last_move;
			for (i = 0; i < 6; i++) {
				last_move = battle_field->moves_used[i];
				switch (last_move) {
					case MOVE_FUSION_FLARE:
					case MOVE_BLUE_FLARE:
						atk_base_power = attack->base_power * 2;
						break;
					default:
						break;
				};
			}
			if (!atk_base_power) {
				atk_base_power = 100;
			}
			break;
		}
		case MOVE_FUSION_FLARE:
		{
			atk_base_power = 0;
			u16 i, last_move;
			for (i = 0; i < 6; i++) {
				last_move = battle_field->moves_used[i];
				switch (last_move) {
					case MOVE_FUSION_BOLT:
					case MOVE_BOLT_STRIKE:
					{
						atk_base_power = attack->base_power * 2;
						break;
					}
					default:
						break;
				};
			}
			if (atk_base_power) {
				atk_base_power = attack->base_power;
			}
			break;
		}
		case MOVE_SOLAR_BEAM:
		{
			u8 weather = get_weather(battle_field);
			if ((weather == WEATHER_SANDSTORM) || (weather == WEATHER_RAIN) ||
			(weather == WEATHER_HEAVY_RAIN) || (weather == WEATHER_HAIL)) {
				atk_base_power = attack->base_power / 2;
				break;
			}
		}
		default:
		{
			atk_base_power = attack->base_power;
			break;
		}
	};


	switch (get_ability(battle_field, attacker, flags)) {
		case ABILITY_TECHNICIAN:
			if (atk_base_power <= 60) {
				atk_base_power = apply_dmg_mod(atk_base_power, 50, 1);
			}
			break;
		case ABILITY_FLARE_BOOST:
			if (attack->is_special || (attacker->ailment == BURN)) {
				atk_base_power = apply_dmg_mod(atk_base_power, 50, 1);
			}
			break;
		case ABILITY_ANALYTIC:
			if (defender->attacked) {
				atk_base_power = apply_dmg_mod(atk_base_power, 30, 1);
			}
			break;
		case ABILITY_RECKLESS:
		{
			u16 attack_id = attack->move_id;
			switch (attack_id) {
				case MOVE_BRAVE_BIRD:
				case MOVE_DOUBLEEDGE:
				case MOVE_FLARE_BLITZ:
				case MOVE_HEAD_CHARGE:
				case MOVE_HEAD_SMASH:
				case MOVE_HIGH_JUMP_KICK:
				case MOVE_JUMP_KICK:
				case MOVE_LIGHT_OF_RUIN:
				case MOVE_SUBMISSION:
				case MOVE_TAKE_DOWN:
				case MOVE_VOLT_TACKLE:
				case MOVE_WOOD_HAMMER:
				case MOVE_WILD_CHARGE:
					atk_base_power = apply_dmg_mod(atk_base_power, 20, 1);
				default:
					break;
			};
			break;
		}
		case ABILITY_IRON_FIST:
		{
			u16 attack_id = attack->move_id;
			switch (attack_id) {
				case MOVE_BULLET_PUNCH:
				case MOVE_COMET_PUNCH:
				case MOVE_DIZZY_PUNCH:
				case MOVE_DRAIN_PUNCH:
				case MOVE_DYNAMIC_PUNCH:
				case MOVE_FIRE_PUNCH:
				case MOVE_FOCUS_PUNCH:
				case MOVE_HAMMER_ARM:
				case MOVE_ICE_PUNCH:
				case MOVE_MACH_PUNCH:
				case MOVE_MEGA_PUNCH:
				case MOVE_METEOR_MASH:
				case MOVE_POWERUP_PUNCH:
				case MOVE_SHADOW_PUNCH:
				case MOVE_SKY_UPPERCUT:
				case MOVE_THUNDER_PUNCH:
					atk_base_power = apply_dmg_mod(atk_base_power, 20, 1);
				default:
					break;
			};
			break;
		}
		case ABILITY_TOXIC_BOOST:
			if (attacker->ailment == POISONNED) {
				atk_base_power = apply_dmg_mod(atk_base_power, 50, 1);
			}
			break;
		case ABILITY_RIVALRY:
			if (attacker->gender == defender->gender) {
				atk_base_power = apply_dmg_mod(atk_base_power, 25, 1);
				break;
			} 
			if (defender->gender == GENDERLESS) {
				break;
			}
			atk_base_power = apply_dmg_mod(atk_base_power, 25, 0);
			break;
		case ABILITY_SAND_FORCE:
		{
			u8 weather = get_weather(battle_field);
			if ((weather == WEATHER_SANDSTORM) && ((attack->type == TYPE_STEEL)
			|| (attack->type == TYPE_ROCK) || (attack->type == TYPE_GROUND))) {
				atk_base_power = apply_dmg_mod(atk_base_power, 30, 1);
			}
			break;
		}
		case ABILITY_HEATPROOF:
			if (attack->type == TYPE_FIRE) {
				atk_base_power = apply_dmg_mod(atk_base_power, 50, 0);
			}
			break;
		case ABILITY_DRY_SKIN:
			if (attack->type == TYPE_FIRE) {
				atk_base_power = apply_dmg_mod(atk_base_power, 25, 1);
			}
			break;
		case ABILITY_SHEER_FORCE:
		{
			u16 i;
			for (i = 0; i < sheer_force_table_size; i++) {
				if (sheer_force_t[i].move_id == attack->move_id) {
					atk_base_power = apply_dmg_mod(atk_base_power, 30, 1);
					break;
				}
			}
			break;
		}
		default:
			break;
	};

	// get item boosts for plates and elemental things
	u16 item_dmg_boost = get_item_modifier(battle_field, attacker, attack, flags);
	if (item_dmg_boost) {
		atk_base_power = apply_dmg_mod(atk_base_power, item_dmg_boost, 1);
	}

	if (attacker->me_first) {
		atk_base_power = apply_dmg_mod(atk_base_power, 50, 1);
	}

	if ((battle_field->modifiers.mud_sport) && (attack->type == TYPE_ELECTRIC)) {
		atk_base_power = apply_dmg_mod(atk_base_power, 67, 0);
	}

	if ((battle_field->modifiers.water_sport) && (attack->type == TYPE_FIRE)) {
		atk_base_power = apply_dmg_mod(atk_base_power, 67, 0);
	}
	return atk_base_power;
}

u32 get_base_attack(struct battle_field *battle_field, u8 attacker_id, u8 defender_id, struct move_table *attack, struct flags *flags) {
	struct battler *attacker = &battle_field->battlers[attacker_id];
	struct battler *defender = &battle_field->battlers[defender_id];
	u32 ad_dmg = 0;

	// get base stat based on attack physical/special
	if (!attack->is_special) {
		ad_dmg = attacker->attack;
	} else {
		ad_dmg = attacker->sp_attack;
	}

	// foul play uses opponent's attack stat
	if (attack->move_id == MOVE_FOUL_PLAY) {
		ad_dmg = defender->attack;
	}

	if ((get_ability(battle_field, attacker, flags) != ABILITY_UNAWARE) &&
	(get_ability(battle_field, defender, flags) != ABILITY_UNAWARE)) {
		// apply stat multipliers
		if (!attack->is_special) {
			ad_dmg = apply_dmg_mod(ad_dmg, ((attacker->attack_multiplier) * 50), 1);
			ad_dmg = apply_dmg_mod(ad_dmg, ((attacker->attack_divider) * 50), 0);
		} else {
			ad_dmg = apply_dmg_mod(ad_dmg, ((attacker->special_attack_multiplier) * 50), 1);
			ad_dmg = apply_dmg_mod(ad_dmg, ((attacker->special_attack_divider) * 50), 0);
		}
	}

	// abilities modifying attack
	u8 c_hp = attacker->current_hp;
	u8 t_hp = attacker->total_hp;
	u8 weather = get_weather(battle_field);

	// thick fat
	if (get_ability(battle_field, defender, flags) == ABILITY_THICK_FAT) {
		if ((attack->type == TYPE_FIRE) || (attack->type == TYPE_ICE)) {
			ad_dmg = apply_dmg_mod(ad_dmg, 50, 0);
		}
	}

	switch (get_ability(battle_field, attacker, flags)) {
		case ABILITY_TORRENT:
			if (((c_hp * 3) <= t_hp) && (attack->type == TYPE_WATER)) {
				ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
			}
			break;
		case ABILITY_GUTS:
			if (attacker->ailment) {
				ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
			}
			break;
		case ABILITY_SWARM:
			if (((c_hp * 3) <= t_hp) && (attack->type == TYPE_BUG)) {
				ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
			}
			break;
		case ABILITY_OVERGROW:
			if (((c_hp * 3) <= t_hp) && (attack->type == TYPE_GRASS)) {
				ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
			}
			break;
		case ABILITY_PLUS:
		{
			u16 i;
			for (i = 0; i < 6; i++) {
				if ((battle_field->battlers[i].ability == ABILITY_MINUS) &&
				(i != attacker_id)) {
					if (attack->is_special) {
						ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
					}
				}
			}
			break;
		}
		case ABILITY_MINUS:
		{
			u16 i;
			for (i = 0; i < 6; i++) {
				if ((battle_field->battlers[i].ability == ABILITY_PLUS) &&
				(i != attacker_id)) {
					if (attack->is_special) {
						ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
					}
				}
			}
			break;
		}
		case ABILITY_BLAZE:
			if (((c_hp * 3) <= t_hp) && (attack->type == TYPE_FIRE)) {
				ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
			}
			break;
		case ABILITY_DEFEATIST:
			if ((c_hp * 2) <= t_hp) {
				ad_dmg = apply_dmg_mod(ad_dmg, 50, 0);
			}
			break;
		case ABILITY_PURE_POWER:
		case ABILITY_HUGE_POWER:
			if (!attack->is_special) {
				ad_dmg = apply_dmg_mod(ad_dmg, 100, 1);
			}
			break;
		case ABILITY_SOLAR_POWER:
			if ((weather == WEATHER_SUNNY) || (weather == WEATHER_HARSH_SUNLIGHT)) {
				if (attack->is_special == 1) {
				ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
				}
			}
			break;
		case ABILITY_HUSTLE:
			if (!attack->is_special) {
				ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
			}
			break;
		case ABILITY_SLOW_START:
			if ((attacker->turns_in_battle <= 5) && (!attack->is_special)) {
				ad_dmg = apply_dmg_mod(ad_dmg, 50, 0);
			}
			break;
		case ABILITY_FLOWER_GIFT:
			if ((get_species(attacker) == POKE_CHERRIM) &&
			((weather == WEATHER_SUNNY) || (weather == WEATHER_HARSH_SUNLIGHT))) {
				if (!attack->is_special == 1) {
					ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
				}
			}
			break;
		default:
			break;
	};

	// flower gift also works for ally cherrim with the ability
	u8 ability = ability_present_ally(battle_field, ABILITY_FLOWER_GIFT, flags);
	if ((ability == (attacker->battle_side + 1)) &&
	((weather == WEATHER_SUNNY) || (weather == WEATHER_HARSH_SUNLIGHT))) {
		// so only cherrim can have flower guard. I don't bother checking :C
		if (!attack->is_special == 1) {
			ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
		}
	}


	// special cases of attack increases with items
	u16 item = get_item(battle_field, attacker, flags);
	u16 species = get_species(attacker);

	// thick club
	if (((species == POKE_CUBONE) || (species == POKE_MAROWAK)) &&
	(item == ITEM_THICK_CLUB)) {
		if (!attack->is_special) {
			ad_dmg = apply_dmg_mod(ad_dmg, 100, 1);
		}
	}

	// deep sea tooth
	if ((species == POKE_CLAMPERL) && (item == ITEM_DEEP_SEA_TOOTH)) {
		if (attack->is_special) {
			ad_dmg = apply_dmg_mod(ad_dmg, 100, 1);
		}
	}

	// light ball
	if ((species == POKE_PIKACHU) && (item == ITEM_LIGHT_BALL)) {
		ad_dmg = apply_dmg_mod(ad_dmg, 100, 1);
	}

	// soul dew
	if ((species == POKE_LATIOS) || (species == POKE_LATIAS)) {
		if ((attack->is_special) && (attacker->item == ITEM_SOUL_DEW)) {
			ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
		}
	}

	// choice band
	if ((item == ITEM_CHOICE_BAND) && (!attack->is_special)) {
		ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
	}

	// choice specs
	if ((item == ITEM_CHOICE_SPECS) && (attack->is_special)) {
		ad_dmg = apply_dmg_mod(ad_dmg, 50, 1);
	}

	return ad_dmg;
}

u32 get_base_defense(struct battle_field *battle_field, u8 attacker_id, u8 defender_id, struct move_table *attack, struct flags *flags) {
	struct battler *attacker = &battle_field->battlers[attacker_id];
	struct battler *defender = &battle_field->battlers[defender_id];
	u16 defense = 0;
	u16 item = get_item(battle_field, attacker, flags);
	u16 species = get_species(attacker);
	u8 special_move = 0;

	switch (attack->move_id) {
		case MOVE_PSYSHOCK:
		case MOVE_SECRET_SWORD:
		case MOVE_PSYSTRIKE:
			defense = get_def(attacker, 0);
			special_move = 1;
			break;
		default:
			if (attack->is_special) {
				defense = get_def(attacker, 1);
			} else {
				defense = get_def(attacker, 0);
			}
			break;
	};

	u8 ability = apply_mold_breaker(battle_field, defender, attacker, flags);
	if ((get_ability(battle_field, defender, flags) != ABILITY_UNAWARE) ||
	(attack->move_id != MOVE_CHIP_AWAY)) {
		// calc stat boosts
		if ((!attack->is_special) || (special_move)) {
			defense = apply_dmg_mod(defense, ((attacker->defense_multiplier) * 50), 1);
			defense = apply_dmg_mod(defense, ((attacker->defense_divider) * 50), 0);
		} else {
			defense = apply_dmg_mod(defense, ((attacker->special_defense_multiplier) * 50), 1);
			defense = apply_dmg_mod(defense, ((attacker->special_defense_divider) * 50), 0);
		}
	}


	// WEATHER_SANDSTORM boosts
	if ((get_weather(battle_field) == WEATHER_SANDSTORM) && (is_type(attacker, TYPE_ROCK)) &&
	(attack->is_special)) {
		defense = apply_dmg_mod(defense, 50, 1);
	}

	// MARVEL scale
	if ((!attack->is_special) && (ability == ABILITY_MARVEL_SCALE)) {
		defense = apply_dmg_mod(defense, 50, 1);
	}

	// cherrim's flower gift
	if ((attack->is_special) && (ability == ABILITY_FLOWER_GIFT)) {
		defense = apply_dmg_mod(defense, 50, 1);
	}

	// deep sea scale
	if ((species == POKE_CLAMPERL) && (item == ITEM_DEEP_SEA_SCALE)) {
		if (attack->is_special) {
			defense = apply_dmg_mod(defense, 100, 1);
		}
	}

	// Metal poweder Ditto is such a snowflake
	if ((species == POKE_DITTO) && (item == ITEM_METAL_POWDER) &&
	(!attacker->transformed)) {
		defense = apply_dmg_mod(defense, 100, 1);
	}

	// eviolite
	if ((!evolution_table[species].method) && (item == ITEM_EVIOLITE)) {
		defense = apply_dmg_mod(defense, 100, 1);
	}

	// Soul dew
	if (((species == POKE_LATIOS) || (species == POKE_LATIAS)) && (item == ITEM_SOUL_DEW)) {
		if (attack->is_special) {
			defense = apply_dmg_mod(defense, 100, 1);
		}
	}
	return defense;
}

u8 weather_dmg_increase(struct battle_field *battle_field, struct battler *attacker, struct move_table *attack) {
	/* 0 = move does 0 dmg, 100 = normal dmg, 150 = 50% increase*/

	u8 weather = get_weather(battle_field);
	switch (weather) {
		case WEATHER_HARSH_SUNLIGHT:
			// water moves fail & fire moves up 50%
			if (attack->type == TYPE_FIRE) {
				return 150;
			}
			if (attack->type == TYPE_WATER) {
				return 0;
			}
			break; //unaffected by sun
		case WEATHER_SUNNY:
			//water moves down, fire moves up 50%
			if (attack->type == TYPE_FIRE) {
				return 150;
			}
			if (attack->type == TYPE_WATER) {
				return 50;
			}
			break;
		case WEATHER_HEAVY_RAIN:
			// water moves 50% up & fire moves fail
			if (attack->type == TYPE_FIRE) {
				return 0;
			}
			if (attack->type == TYPE_WATER) {
				return 150;
			}
			break;
		case WEATHER_RAIN:
			// water moves up & fire moves down 50%
			if (attack->type == TYPE_FIRE) {
				return 50;
			}
			if (attack->type == TYPE_WATER) {
				return 150;
			}
			break;
		case WEATHER_MYSTERIOUS_AIR_CURRENT:
			// if flying type, deal half dmg is super-effective
			if (is_type(attacker, TYPE_FLYING)) {
				if (not_immune(attacker, attack->type) > 100) {
					return 50;
				}
			}
			break;
		default:
			return 100;
	};
	return 100;
}

u8 get_crit_dmg(struct battle_field *battle_field, struct battler *attacker, struct move_table *attack, struct flags *flags) {
	// crit immunities: shell armor, battle armor, safeguard-type moves
	if (((get_ability(battle_field, attacker, flags) == ABILITY_SHELL_ARMOR) ||
	(get_ability(battle_field, attacker, flags) == ABILITY_BATTLE_ARMOR)) ||
	(battle_field->modifiers.safeguard[2 * attacker->battle_side])) {
		return 100;
	} else {
		u8 crit_modifier = attacker->crit_multiplier;

		// high crit moves, boost by 1
		if (attack->crit_chance) {
			crit_modifier ++;
		}

		// items boosting crit
		u16 item = get_item(battle_field, attacker, flags);
		u16 species = get_species(attacker);
		switch (item) {
			case ITEM_SCOPE_LENS:
			case ITEM_RAZOR_CLAW:
				crit_modifier ++;
				break;
			case ITEM_STICK:
				if (species == POKE_FARFETCHD) {
					crit_modifier += 2;
				}
				break;
			case ITEM_LUCKY_PUNCH:
				if (species == POKE_CHANSEY) {
					crit_modifier += 2;
				}
				break;
			default:
				break;
		};

		// calculate crit damage given chance
		u8 denominator = 0;
		switch (crit_modifier) {
			case 1:
				denominator = 16;
				break;
			case 2:
				denominator = 8;
				break;
			case 3:
				denominator = 2;
				break;
			default:
				denominator = 1;
				break;
		};
		u8 crit_chance = random(denominator);
		if (crit_chance) {
			return 50;
		}
	}
	return 0;
}

u8 get_STAB(struct battle_field *battle_field, struct battler *attacker, struct move_table *attack, struct flags *flags) {
	u16 i;
	for (i = 0; i < 3; i++) {
		if ((attack->type) == attacker->types[i]){
			if (get_ability(battle_field, attacker, flags) == ABILITY_ADAPTABILITY) {
				return 100;
			} else {
				return 50;
			}
		}
	}
	return 0; // no stab
}

u8 reflect_dmg(struct battle_field *battle_field, struct battler *defender, struct move_table *attack) {
	if (battle_field->modifiers.reflect[defender->battle_side * 2] && attack->is_special) {
		return 50;
	}
	return 0;
}

u8 light_screen_dmg(struct battle_field *battle_field, struct battler *defender, struct move_table *attack) {
	if (battle_field->modifiers.lightscreen[defender->battle_side * 2] && attack->is_special) {
		return 50;
	}
	return 0;
}

u8 berry_reduction(struct battle_field *battle_field, struct battler *attacker, struct move_table *attack, struct flags *flags) {
	u16 item = get_item(battle_field, attacker, flags);
	switch (item) {
		case ITEM_OCCA_BERRY:
			if (attack->type == TYPE_FIRE) {
				return 50;
			}
			break;
		case ITEM_PASSHO_BERRY:
			if (attack->type == TYPE_WATER) {
				return 50;
			}
			break;
		case ITEM_WACAN_BERRY:
			if (attack->type == TYPE_ELECTRIC) {
				return 50;
			}
			break;
		case ITEM_RINDO_BERRY:
			if (attack->type == TYPE_GRASS) {
				return 50;
			}
			break;
		case ITEM_YACHE_BERRY:
			if (attack->type == TYPE_ICE) {
				return 50;
			}
			break;
		case ITEM_CHOPLE_BERRY:
			if (attack->type == TYPE_FIGHTING) {
				return 50;
			}
			break;
		case ITEM_KEBIA_BERRY:
			if (attack->type == TYPE_POISON) {
				return 50;
			}
			break;
		case ITEM_SHUCA_BERRY:
			if (attack->type == TYPE_GROUND) {
				return 50;
			}
			break;
		case ITEM_COBA_BERRY:
			if (attack->type == TYPE_FLYING) {
				return 50;
			}
			break;
		case ITEM_PAYAPA_BERRY:
			if (attack->type == TYPE_PSYCHIC) {
				return 50;
			}
			break;
		case ITEM_TANGA_BERRY:
			if (attack->type == TYPE_BUG) {
				return 50;
			}
			break;
		case ITEM_CHARTI_BERRY:
			if (attack->type == TYPE_ROCK) {
				return 50;
			}
			break;
		case ITEM_KASIB_BERRY:
			if (attack->type == TYPE_GHOST) {
				return 50;
			}
			break;
		case ITEM_HABAN_BERRY:
			if (attack->type == TYPE_DRAGON) {
				return 50;
			}
			break;
		case ITEM_COLBUR_BERRY:
			if (attack->type == TYPE_DARK) {
				return 50;
			}
			break;
		case ITEM_BABIRI_BERRY:
			if (attack->type == TYPE_STEEL) {
				return 50;
			}
			break;
		case ITEM_ROSELI_BERRY:
			if (attack->type == TYPE_FAIRY) {
				return 50;
			}
			break;
		default:
			break;
	};
	return 0;
}

u32 damage_calculator(struct battle_field *battle_field, u8 attacker_id, u8 defender_id, struct move_table *attack, u8 ability_f, u16 item_f, u8 pursuit_f) {
	/*
		Assumptions about caller:
		1) defender/attacker->damage_taken is reset if not in bide
		2) correct defender is choosen depending on move
		3) move is not a status move
		4) User immunities (unrelated to type) are checked prior i.e currently flying

	*/

	struct battler *attacker = &battle_field->battlers[attacker_id];
	struct battler *defender = &battle_field->battlers[defender_id];

	/* Check if special case dmg move*/
	switch (attack->move_id) {
		case MOVE_PSYWAVE:
			if (not_immune(defender, attack->type)) {
				// (x + 50) * level st. 0<= x <= 100
				return ((random(100) + 50) / 100) * attacker->level;
			} else {
				return 0;
			}
		case MOVE_NIGHT_SHADE:
		case MOVE_SEISMIC_TOSS:
			if (not_immune(defender, attack->type)) {
				// does attacker's level in dmg
				return attacker->level;
			} else {
				return 0;
			}
		case MOVE_SONIC_BOOM:
			if (not_immune(defender, attack->type)) {
				// always 20 dmg
				return 20;
			} else {
				return 0;
			}
		case MOVE_SUPER_FANG:
			if (not_immune(defender, attack->type)) {
				// half the defender's HP
				return apply_dmg_mod(defender->current_hp, 50, 0);
			} else {
				return 0;
			}
		case MOVE_DRAGON_RAGE:
			if (not_immune(defender, attack->type)) {
				// always 40 dmg
				return 40;
			} else {
				return 0;
			}
		case MOVE_ENDEAVOR:
			if (not_immune(defender, attack->type)) {
				// do difference between attackers HP and Defenders HP.
				u16 attacker_hp_current = attacker->current_hp;
				u16 defender_hp_current = defender->current_hp;
				if (attacker_hp_current < defender_hp_current) {
					return defender_hp_current - attacker_hp_current;
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		case MOVE_FINAL_GAMBIT:
			if (not_immune(defender, attack->type)) {
				// do amount of health of attacker
				return attacker->current_hp;
			} else {
				return 0;
			}
		case MOVE_COUNTER:
			if (not_immune(defender, attack->type)) {
				// do double damage of hit by attack
				if ((!attacker->damage_taken_type) &&
				(attacker->damage_taken)) {
					return (attacker->damage_taken) * 2;
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		case MOVE_MIRROR_COAT:
			if (not_immune(defender, attack->type)) {
				// do double damage of hit by special attack
				if ((attacker->damage_taken_type) && (attacker->damage_taken)) {
					return (attacker->damage_taken) * 2;
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		case MOVE_METAL_BURST:
			if (not_immune(defender, attack->type)) {
				// fails if user took no dmg, or is faster
				if (!attacker->damage_taken) {
					return 0;
				} else {
					// 50% increased damage
					return apply_dmg_mod(attacker->damage_taken, 50, 1);
				}
			} else {
				return 0;
			}
		case MOVE_BIDE:
			if (not_immune(defender, attack->type)) {
				switch (attacker->bide) {
				case 1:
					// attacker unleashed bide dmg
					attacker->bide -= 1;
					if (attacker->damage_taken == 0) {
						return 0;
					} else {
						return attacker->damage_taken;
					}
				case 0:
					// freshly used bide
					attacker->damage_taken = 0;
					attacker->bide = 2;
					return 0;
				default:
					// charging up bide still
					attacker->bide -= 1;
					return 0;
				};
			} else {
				return 0;
			}
		default:
			break;
	};

	struct flags *flags = (struct flags*) malloc(sizeof(struct flags));

	// set up flags
	if (attacker->current_hp == attacker->total_hp) {
		flags->health_was_full = 1; // full hp flag
	}
	flags->abilities_disabled = ability_f;
	flags->pursuit_flag = pursuit_f; // pursuit will be 2x dmg
	flags->items_disabled = item_f; // disabled items battle mode

	u32 current_dmg = 1;
	/* Calculate level scaling */
	current_dmg = (apply_dmg_mod((2 * attacker->level), 20, 0)) + 2;

	/* Calculate base damage */
	current_dmg *= get_base_power(battle_field, attacker_id, defender_id, attack, flags);

	/* Calculate attack stat */
	current_dmg = get_base_attack(battle_field, attacker_id, defender_id, attack, flags);

	/* Calculate defense stat */
	u32 base_def = get_base_defense(battle_field, attacker_id, defender_id, attack, flags);
	current_dmg = current_dmg / base_def;
	current_dmg = (current_dmg / 50) + 2;

	// double/triple battle reduce multi-target dmg
	if ((battle_field->battle_type != SINGLE) && (battle_field->battle_type != HORDE)) {
		if ((attack->affected_targets == 0x20) || (attack->affected_targets == 0x8)) {
			current_dmg = apply_dmg_mod(current_dmg, 75, 0);
		}
	}

	// get weather boosts
	u32 other_dmg = weather_dmg_increase(battle_field, attacker, attack);
	switch (other_dmg) {
		case 50:
			current_dmg = apply_dmg_mod(current_dmg, other_dmg, 0);
			break;
		case 150:
			current_dmg = apply_dmg_mod(current_dmg, (other_dmg - 100), 1);
			break;
		default:
			break;
	};

	// get and apply crit dmg
	u8 crit_dmg = get_crit_dmg(battle_field, attacker, attack, flags);
	if (crit_dmg) {
		flags->crit_flag = 1;
		current_dmg = apply_dmg_mod(current_dmg, crit_dmg, 1);
	}

	// Alter by random factor
	current_dmg *= (100 - random(15));
	current_dmg = current_dmg / 100;

	// get and apply STAB
	u8 stab_dmg = get_STAB(battle_field, attacker, attack, flags);
	if (stab_dmg) {
		current_dmg = apply_dmg_mod(current_dmg, stab_dmg, 1);
	}

	// get type damage boosting
	u8 type_dmg = not_immune(defender, attack->type);
	if (!type_dmg) {
		return 0;
	}

	if (type_dmg < 100) {
		// not very effective
		flags->effectiveness = 1;
		current_dmg = apply_dmg_mod(current_dmg, type_dmg, 0);
	}

	if (type_dmg > 100) {
		// super effective
		flags->effectiveness = 2;
		current_dmg = apply_dmg_mod(current_dmg, type_dmg, 1);
	}

	// check burn
	if ((attacker->ailment == BURN) || (!attack->is_special)) {
		if ((attack->move_id != MOVE_FACADE) && (attacker->ability != ABILITY_GUTS)) {
			current_dmg = apply_dmg_mod(current_dmg, 50, 0);
		}
	}

	// make sure dmg atleast 1
	if (current_dmg < 1) {
		current_dmg = 1;
	}

	/* Calculate and apply final modifier */

	// get reflect/light screen reduction
	if ((flags->crit_flag) && (get_ability(battle_field, attacker, flags) == ABILITY_INFILTRATOR)) {
		u8 screen = reflect_dmg(battle_field, defender, attack);
		if (screen) {
			current_dmg = apply_dmg_mod(current_dmg, screen, 0);
		} else {
			screen = light_screen_dmg(battle_field, defender, attack);
			if (screen) {
				current_dmg = apply_dmg_mod(current_dmg, screen, 0);
			}
		}
	}

	// get multiscale reduction
	if ((flags->health_was_full) && (get_ability(battle_field, defender, flags) == ABILITY_MULTISCALE)) {
		current_dmg = apply_dmg_mod(current_dmg, 50, 0);
	}

	// tinted lens boost
	if ((get_ability(battle_field, attacker, flags) == ABILITY_TINTED_LENS) && (flags->effectiveness)) {
		current_dmg = apply_dmg_mod(current_dmg, 100, 1);
	}

	// friend guard
	if (((battle_field->ally[defender_id]) + 1) ==
	(ability_present_ally(battle_field, ABILITY_FRIEND_GUARD, flags))) {
		current_dmg = apply_dmg_mod(current_dmg, 25, 0);
	}

	// sniper ability
	if ((get_ability(battle_field, attacker, flags) == ABILITY_SNIPER) &&
	(flags->crit_flag)) {
		current_dmg = apply_dmg_mod(current_dmg, 50, 1);
	}

	// solid rock & filter
	if (flags->effectiveness == 2) {
		u8 ability = get_ability(battle_field, defender, flags);
		if ((ability == ABILITY_FILTER) || (ability == ABILITY_SOLID_ROCK)) {
			current_dmg = apply_dmg_mod(current_dmg, 75, 0);
		}
	}

	u16 item = get_item(battle_field, attacker, flags);
	// Metronome item
	if (item == ITEM_METRONOME_item) {
		current_dmg = apply_dmg_mod(current_dmg, ((attacker->metronome) * 20), 1);
	}

	// Expert belt item
	if ((item == ITEM_EXPERT_BELT) && (flags->effectiveness == 2)) {
		current_dmg = apply_dmg_mod(current_dmg, 20, 1);
	}
	// Life orb
	if (item == ITEM_LIFE_ORB) {
		current_dmg = apply_dmg_mod(current_dmg, 30, 1);
	}

	// Berries which reduce super-effective dmg
	if ((get_ability(battle_field, defender, flags) != ABILITY_KLUTZ) &&
	(get_ability(battle_field, attacker, flags) != ABILITY_UNNERVE)) {
		if (flags->effectiveness == 2) {
			// can eat the berry
			if (berry_reduction(battle_field, attacker, attack, flags)) {
				current_dmg = apply_dmg_mod(current_dmg, 50, 0);
			}
		} else {
			if ((attack->type == TYPE_NORMAL) && (get_item(battle_field, defender, flags) == ITEM_CHILAN_BERRY)) {
				current_dmg = apply_dmg_mod(current_dmg, 50, 0);
			}
		}
	}

	// Move used with double power trigger
	switch (attack->move_id) {
		case MOVE_EARTHQUAKE:
		case MOVE_MAGNITUDE:
			if (defender->dig) {
				current_dmg = apply_dmg_mod(current_dmg, 100, 1);
			}
			break;
		case MOVE_SURF:
		case MOVE_WHIRLPOOL:
			if (defender->dive) {
				current_dmg = apply_dmg_mod(current_dmg, 100, 1);
			}
			break;
		case MOVE_STOMP:
		case MOVE_STEAMROLLER:
		case MOVE_BODY_SLAM:
		case MOVE_DRAGON_RUSH:
		case MOVE_FLYING_PRESS:
		case MOVE_PHANTOM_FORCE:
			if (defender->minimize) {
				current_dmg = apply_dmg_mod(current_dmg, 100, 1);
			}
			break;
		default:
			break;
	};

	free(flags);
	return current_dmg;
}
