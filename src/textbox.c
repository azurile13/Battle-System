#include "types.h"
#include "ROM_tables.h"
#include "lcd.h"
#include "engine/callback.h"
#include "defines.h"

void box_related_one(u8, u8, u8* string, u8, u8, u8, u8, u8);
void rboxid_to_vram(u8, u8);
void box_curved(u8, u8);
void sub_080F696C(u8, u32, u8, u8, u8);
void sub_08002040(u8, u32, u8, u8);
void gpu_sync_bg_show(u8);
void bgid_send_tilemap(u8);
void rboxid_clear_pixels(u8, u8);
void rboxid_08003FA0(u8);
void sub_0800F34C(void);
void rboxid_tilemap_update(u8);
void rboxid_upload_a(u8, u8);
u8 sav2_get_text_speed(void);
void sub_8002C48(u8, u8, u8*, u8, u8, u8, u8);
void sub_8150048(u8, u8, u8);
void battle_show_message(char [], u8);
u16 remoid_a_pressed_maybe(u8);

char string_buffer[1000];

void display_textbox() {
	gpu_sync_bg_show(0);
}

void waitbutton_text(u8 task_id) {
	if (!remoid_a_pressed_maybe(0)) {
		superstate.multi_purpose_state_tracker = tasks[task_id].priv[0];
		task_del(task_id);
	}
}

void textbox_set_text(char string[], u8 font, u8 text_bg, u8 text_colour, u8 shadow, u8 text_bg_box) {
	/*textflags.unknown[0] = textflags.unknown[0] | 1;
	if (text_bg_box) {
		rboxid_clear_pixels(0, 0xFF); // transparent
	} else {
		rboxid_clear_pixels(0, 0x11); // white
	}

	//sub_8002C48(0, 3, string, 0, 2, 0xFF, 0);
	//sub_8150048(0, 0x14, 0xC);
	//rboxid_tilemap_update(0);
	//rboxid_upload_a(0, 3);
	rboxid_08003FA0(0);
	rboxid_to_vram(0, 0);
	box_related_one(0, font, string, sav2_get_text_speed(), 0, text_colour, text_bg, shadow);*/
	battle_show_message(string, 0x0);
	display_textbox();
}


void create_battle_box() {
	sub_0800F34C();
	lz77_uncomp_vram((void *)0x8D00000, (void *)0x6000000);
	sub_080F696C(0, 0x8D00000, 0, 0, 0);
	sub_08002040(0, 0x8D0051C, 0, 0);
	bgid_send_tilemap(0);
	pal_decompress_slice_to_faded_and_unfaded((void *)0x8D004D8, 0, 0x40);
}

void create_curved_box() {
	box_curved(0, 1);
}

//[class] [name] would like to battle
// Wild [name] appeared

void quick_setup_textbox(u8 string_id) {
	create_battle_box();
	display_textbox();
	//textbox_set_text((u8 *[])0x81C55C9, 1, 0, 1, 3, 1);
	//textbox_set_text(battle_strings.string[string_id], 1, 0, 1, 3, 1);

}

/*
	Pick and use battle start message

*/


/* TODO

- Fight, Bag, Pkmn, Run menu
*/











