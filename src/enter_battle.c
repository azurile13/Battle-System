#include "types.h"
#include "ROM_tables.h"
#include "lcd.h"
#include "engine/callback.h"
#include "oam.c"
#include "defines.h"

#define PLAYER1_OAM_START_POS_X 0x130
#define PLAYER1_OAM_START_POS_Y 0x50

#define OPPONENT1_OAM_START_POS_X 0x130
#define OPPONENT1_OAM_START_POS_Y 0x130

/* 
 *
 * Player side, trainer enter field OAM callback related 
 *
 */

 
// Player single obj callbacks below
void move_off_and_delete(struct object *obj) {
	if (obj->x > 0x0) {
		obj->x--;
	} else {
		obj_delete(obj);
	}
	return;
}

void obj_callback_slide_left(struct object *obj) {
	if (!obj->private[2]) {
		if (obj->x > 0x70 - (obj->private[1] * 0x2A)) {
			obj->x--;
		} else {
			if (obj->private[0] == 1) {
				u8 obj_id = obj_id_from_obj(obj);
				do_anim(obj_id);
				obj->callback = (object_callback) move_off_and_delete;
			}
		}
	} else {
		obj->private[2]--;
	}
	return;
}


// obj callbacks for triples
void obj_callback_slide_right_triple(struct object *obj) {
	if (obj->private[1] > 5) {
		if (obj->x < (0x300 - (((obj->private[1] - 5) * 0x20)) - 0x10)) {
			obj->x++;
		}
	} else {
		if (obj->x < (0x300 - (obj->private[1] * 0x30))) {
			obj->x++;
		}
	}
	switch(obj->private[1]) {
		case 1:
		case 6:
			obj->y = 0x35;
			break;
		case 2:
		case 7:
			obj->y = 0x25;
			break;
		case 3:
		case 8:
		default:
			obj->y = 0x35;
			break;
	};
}


void obj_callback_hide_triple(struct object *obj) {
	if (obj->private[0] > 0) {
		obj->x = 0x1E0;
		obj->y = 0x130;
		obj->private[0]--;
	} else {
		//obj->x = 0x0;
		obj->y = 0x30;
		obj->callback = obj_callback_slide_right_triple;
	}
	return;
}


// double & single battle obj callbacks
void obj_callback_slide_right_double(struct object *obj) {
	if (obj->private[1] < 5) {
		if (obj->x < (0x300 - (obj->private[1] * 0x30))) {
				obj->x++;
		}
	} else {
		if (obj->x < (0x300 - 0x45)) {
				obj->x++;
		}
	}
}


void obj_callback_hide_double(struct object *obj) {
	if (obj->private[0] > 0) {
		obj->x = 0x1E0;
		obj->y = 0x130;
		obj->private[0]--;
	} else {
		//obj->x = 0x0;
		obj->y = 0x30;
		obj->callback = obj_callback_slide_right_double;
	}
	return;
}



void obj_callback_slide_right(struct object *obj) {
}

void enter_field_playerside_oam(u8 type, struct battle_config* b_config) {
	u8 gender = saveblock2.sav2->gender;
	type = DOUBLE_TWO_AI;
	b_config->ally_backsprite = 3;
	switch (type) {
		case SINGLE_TRAINER:
		case SINGLE_WILD:
		case DOUBLE_WILD:
		case TRIPLE_WILD:
		case TRIPLE_TRAINER:
		case HORDE_TRAINER:
		case HORDE_WILD:
		case DOUBLE_TRAINER:
			{
			u8 id =	oam_trainer_back(gender, PLAYER1_OAM_START_POS_X, PLAYER1_OAM_START_POS_Y, (object_callback) obj_callback_slide_left);
			objects[id].private[1] = 1;
			objects[id].private[2] = 0;
			break;
			}
		case DOUBLE_TWO_AI:
			{
			u8 id = oam_trainer_back(gender, PLAYER1_OAM_START_POS_X, PLAYER1_OAM_START_POS_Y, (object_callback) obj_callback_slide_left);
			objects[id].private[1] = 2;
			objects[id].private[2] = 0;
			id = oam_trainer_back(b_config->ally_backsprite, PLAYER1_OAM_START_POS_X, PLAYER1_OAM_START_POS_Y, (object_callback) obj_callback_slide_left);
			objects[id].private[1] = 1;
			objects[id].private[2] = 0x30;
			break;
			}
	};
	
	
	return;
}

void enter_field_opponent(u8 type, struct battle_config* b_config) {
	b_config->opp_id[0] = 0x50;
	b_config->opp_id[1] = 0x51;
	b_config->opp_id[2] = 0x52;
	b_config->opp_id[3] = 0x53;
	b_config->opp_id[4] = 0x54;
	type = SINGLE_WILD;
	switch (type) {
		case SINGLE_TRAINER:
		case DOUBLE_TRAINER: // one trainer, two pkmn
			{
			u8 id = oam_trainer_front(b_config->opp_id[0], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
			objects[id].private[0] = 0x20;
			objects[id].private[1] = 1;
			break;
			}
		case DOUBLE_WILD:
			{
			u8 id = oam_pkmn_front(b_config->opp_id[0], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
			objects[id].private[0] = 0x20;
			objects[id].private[1] = 1;
			id = oam_pkmn_front(b_config->opp_id[0], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
			objects[id].private[0] = 0x50;
			objects[id].private[1] = 2;
			break;
			}
		case DOUBLE_TWO_AI:
			{
			u8 id = oam_trainer_front(b_config->opp_id[0], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
			objects[id].private[0] = 0x20;
			objects[id].private[1] = 1;
			id = oam_trainer_front(b_config->opp_id[1], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
			objects[id].private[0] = 0x50;
			objects[id].private[1] = 2;
			break;
			}
		case TRIPLE_WILD:
			{
			u8 id = oam_pkmn_front(b_config->opp_id[0], 0, OPPONENT1_OAM_START_POS_X - 20, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
			objects[id].private[0] = 0x20;
			objects[id].private[1] = 1;
			id = oam_pkmn_front(b_config->opp_id[1], 0, OPPONENT1_OAM_START_POS_X - 30, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
			objects[id].private[0] = 0x50;
			objects[id].private[1] = 2;
			id = oam_pkmn_front(b_config->opp_id[2], 0, OPPONENT1_OAM_START_POS_X - 40, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
			objects[id].private[0] = 0x80;
			objects[id].private[1] = 3;
			break;
			}
		case TRIPLE_TRAINER:
			{
			u8 id = oam_trainer_front(b_config->opp_id[0], OPPONENT1_OAM_START_POS_X - 20, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
			objects[id].private[0] = 0x20;
			objects[id].private[1] = 6;
			id = oam_trainer_front(b_config->opp_id[1], OPPONENT1_OAM_START_POS_X - 25, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
			objects[id].private[0] = 0x40;
			objects[id].private[1] = 7;
			id = oam_trainer_front(b_config->opp_id[2], OPPONENT1_OAM_START_POS_X - 0x2F, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_triple);
			objects[id].private[0] = 0x60;
			objects[id].private[1] = 8;
			break;
			}
		case HORDE_WILD:
			{
			u8 id = oam_pkmn_front(b_config->opp_id[0], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
			objects[id].private[0] = 0x20;
			id = oam_pkmn_front(b_config->opp_id[1], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
			objects[id].private[0] = 0x30;
			id = oam_pkmn_front(b_config->opp_id[2], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
			objects[id].private[0] = 0x40;
			id = oam_pkmn_front(b_config->opp_id[3], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
			objects[id].private[0] = 0x50;
			id = oam_pkmn_front(b_config->opp_id[4], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
			objects[id].private[0] = 0x60;
			break;
			}
		case HORDE_TRAINER:
			{
			u8 id = oam_trainer_front(b_config->opp_id[0], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
			objects[id].private[0] = 0x20;
			id = oam_trainer_front(b_config->opp_id[1], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
			objects[id].private[0] = 0x30;
			id = oam_trainer_front(b_config->opp_id[2], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
			objects[id].private[0] = 0x40;
			id = oam_trainer_front(b_config->opp_id[3], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
			objects[id].private[0] = 0x50;
			id = oam_trainer_front(b_config->opp_id[4], OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_slide_right);
			objects[id].private[0] = 0x60;
			break;
			}
		case SINGLE_WILD:
		default:
			{
			u8 id = oam_pkmn_front(b_config->opp_id[0], 0, OPPONENT1_OAM_START_POS_X, OPPONENT1_OAM_START_POS_Y, (object_callback) obj_callback_hide_double);
			objects[id].private[0] = 0x20;
			objects[id].private[1] = 6;
			break;
			}
	};
	return;
};