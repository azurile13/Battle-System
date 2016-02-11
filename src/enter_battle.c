#include "types.h"
#include "ROM_tables.h"
#include "lcd.h"
#include "engine/callback.h"
#include "oam.c"
#include "defines.h"


/*
 *
 * Player side, trainer enter field OAM callback related
 *
 */


// Player single obj callbacks below
void move_off_and_delete(struct object *obj) {
	if (obj->x < 0x1E0) {
		obj->x++;
	} else {
		obj_delete_and_free_tiles(obj);
	}
	return;
}

void move_off_and_delete_p(struct object *obj) {
	if (obj->x > 0x0) {
		obj->x--;
	} else {
		obj_delete_and_free_tiles(obj);
	}
	return;
}

void obj_callback_slide_left(struct object *obj) {
	if (!obj->private[2]) {
		if (obj->x > 0x60 - (obj->private[1] * 0x2A)) {
			obj->x -= 2;
		} else {
			obj->private[0] = 2;
		}
	} else {
		obj->private[2]--;
	}
	return;
}


void obj_callback_slide_left_player(struct object *obj) {
	return;
}


// obj callbacks for triples
void obj_callback_slide_right_triple(struct object *obj) {
	if (obj->private[1] > 5) {
		if (obj->x < (0x300 - (((obj->private[1] - 5) * 0x20)) - 0x10)) {
			obj->x+=2;
		} else {
			obj->private[0] = 2;
		}
	} else {
		if (obj->x < (0x300 - (obj->private[1] * 0x30))) {
			obj->x+=2;
		} else {
			obj->private[0] = 2;
		}
	}
	switch(obj->private[1]) {
		case 1:
		case 6:
			obj->y = 0x28 + enemy_y_table[obj->private[4]].height;
			break;
		case 2:
		case 7:
			obj->y = 0x28 + enemy_y_table[obj->private[4]].height;
			break;
		case 3:
		case 8:
		default:
			obj->y = 0x28 + enemy_y_table[obj->private[4]].height;
	};

}


void obj_callback_hide_triple(struct object *obj) {
	if (obj->private[0] > 0) {
		obj->x = 0x1E0;
		obj->y = 0x130;
		obj->private[0]--;
	} else {
		//obj->x = 0x0;
		obj->y = 0x28 + enemy_y_table[obj->private[4]].height;
		obj->callback = obj_callback_slide_right_triple;
	}
	return;
}


// double & single battle obj callbacks
void obj_callback_slide_right_double(struct object *obj) {
	if (obj->private[1] < 5) {
		if (obj->x < (0x300 - (obj->private[1] * 0x30))) {
				obj->x+= 2;
		} else {
			obj->private[0] = 2;
		}
	} else {
		if (obj->x < (0x300 - 0x45)) {
				obj->x+= 2;
		} else {
			obj->private[0] = 2;
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
		obj->y = 0x28 + enemy_y_table[obj->private[4]].height;
		obj->callback = obj_callback_slide_right_double;
	}
	return;
}



void obj_callback_slide_right(struct object *obj) {
	if (obj->private[1] > obj->private[0]) {
		obj->y = 0x28 + enemy_y_table[obj->private[4]].height;
		if (obj->private[2] < (0x120 - obj->private[0])) {
			obj->x += 2;
			obj->private[2] += 2;

		} else {
			obj->private[0] = 2;
			obj->callback = (object_callback) oac_nullsub_0;
		}
	} else {
		obj->x = 0x1E0;
		obj->y = 0x1E0;
		obj->private[1] += 2;
	}

}
