#ifndef MULTIPURPOSE_H
#define MULTIPURPOSE_H

#include "ROM_tables.h"
#include "battle_engine_resource.h"
#include "./engine/callback.h"
#include "./engine/objects.h"

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
void rboxid_clear_pixels(u8, u8);
void rboxid_08003FA0(u8);
void rboxid_to_vram(u8, u8);
void something_08050DE1(void);
void gpu_tile_obj_alloc_tag_and_upload(u32);
void gpu_tile_obj_alloc_tag_and_apply(u32);
void gpu_sync_bg_show(u8);
void something_08050969(u8);
u8 gpu_pal_alloc_new(u16);
u16 species_to_dex_index(u16);
void *memset(void *s, int c, u32 n);
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

#endif /* MULTIPURPOSE_H */
