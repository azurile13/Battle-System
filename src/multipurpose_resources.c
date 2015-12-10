#include "ROM_tables.h"
#include "battle_engine_resource.h"
#include "./engine/callback.h"

/*
 * Data
 */
 
 struct sprite {
	u32 y : 8;
	u32 rotscale : 1;
	u32 double_size;
	u32 obj_mode : 2;
	u32 mosaic : 1;
	u32 colors : 1;
	u32 shape : 2;

	u32 x : 9;
	union {
		u32 rotscale_param : 5;
		struct {
			u32 unused : 3;
			u32 hflip : 1;
			u32 vflip : 1;
		};
	};
	u32 size : 2; 

	u16 char_name : 10;
	u16 priority : 2;
	u16 palette : 4;
	
	u16 rotscale_info; 
};

 struct super {
	super_callback callback1;
	super_callback callback2;
	super_callback callback2backup;
	super_callback callback5_vblank;
	super_callback hblank_callback;
	u32 field_14;
	u32 field_18;
	u32 bit_to_wait_for;
	u32 *ptr_vblank_counter;
	u32 field_24;
	u16 buttons_held;
	u16 buttons_new;
	u16 buttons_held_remapped;
	u16 buttons_new_remapped;
	u16 buttons_new_and_key_repeat;
	u32 keypad_countdown;
	u32 unused_padding;
	struct sprite sprites[128];
	u8 multi_purpose_state_tracker;
	u8 gpu_sprites_upload_skip;
 };

/* 
 * declarations 
 */
 
u16 rand(void);
u32 divmod_remainder(u32, u32);
u32 div(u32, u32);
void *malloc(u32 size);
void free(void *ptr);
u32 get_attribute(struct pokemon *, u8);
void fade_and_return_progress_probably(void);
void obj_sync_something(void);
void objc_exec(void);
void textbox_something();

extern struct super superstate;

/*
 * Functions
 */
 
u32 random(u32 end) {
	u32 result = (u32) rand();
	return (divmod_remainder(result, end));
}

u32 get_attr(struct pokemon *pkmn, u8 arg) {
	return (get_attribute(pkmn, arg));
}