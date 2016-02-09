#include "types.h"
#include "ROM_tables.h"
#include "lcd.h"
#include "engine/callback.h"
#include "multipurpose_resources.h"
#include "engine/objects.h"

void LZ77UnCompAnyRAM(u8 *src, u8 *dst, u8 id);
void uns_builder_assign_animtable1(u8);
u8 oamt_spawn_poke_or_trainer_picture(u16, u32, u32, u8, u16, u16, u8, u16, u8, u8);
void sub_08034750(u8, u8);
void template_build_for_pokemon_or_trainer(u8, u8);
object_callback oac_poke_opponent();
struct sprite *oam_poke_ally;
object_callback oac_nullsub_0();
struct rotscale_frame **anim_082347F8;


void do_anim(u8 obj_id) {
	void obj_anim_image_start(struct object*, u8);
	obj_anim_image_start(&objects[obj_id], 1);
	return;
}


void obj_callback_slide_up(struct object *obj) {
	if (obj->y < 0x100) {
		obj->y --;
	}
	return;
}


void obj_callback_slide_down(struct object *obj) {
	if (obj->y > 0x50) {
		obj->y++;
	}
	return;
}

void task_obj_del_delayed(u8 task_id) {

	if (tasks[task_id].priv[1] <= 1) {
		obj_delete_and_free_tiles(&objects[tasks[task_id].priv[0]]);
		task_del(task_id);
	} else {
		tasks[task_id].priv[1]--;
	}
	return;
}

void obj_del_delayed(u8 obj_id, u16 delay) {
	u8 task_id = task_add(task_obj_del_delayed, 0x1);
	tasks[task_id].priv[0] = obj_id;
	tasks[task_id].priv[1] = delay;
}

u8 create_oam(void *pal, u8 *img, u32 img_size, u16 x, u16 y, u16 id, object_callback cb) {

	// allocate palette
	u8 pal_id = gpu_pal_alloc_new((id *0x10) + 0x100);
	pal_decompress_slice_to_faded_and_unfaded(pal, (0x100 + pal_id *0x10), 0x20);
	
	// set up objtemplate structure
	struct objtemplate *oam_template = (struct objtemplate*)malloc(sizeof(struct objtemplate));
	oam_template->tiles_tag = 0xFFFF;
	oam_template->pal_tag = (id *0x10) + 0x100;
	oam_template->oam = uns_table_pokemon_trainer[0].oam;
	oam_template->rotscale = uns_table_pokemon_trainer[0].rotscale;
	oam_template->callback = cb;
	oam_template->animation = meta_animtable_trainer.trainer_anim[0];
	
	// send gfx to vram
	u8 *graphic_pointer = (u8 *)malloc(img_size);
	lz77_uncomp_vram(img, (void *)graphic_pointer);
	
	// set up resource struct for Objtemplate
	struct resource *resource = (struct resource*)malloc(sizeof(struct resource));
	resource->graphics = graphic_pointer;
	resource->size = img_size;
	oam_template->graphics = resource;
	
	// create OAM
	u8 obj_id = template_instanciate_forward_search(oam_template, x, y, 0x1E);
	return obj_id;
}

u8 oam_pkmn_front(u16 species, u8 shinyness, u16 x, u16 y, object_callback cb) {
	void *pal;
	
	// use shiny pal if shiny
	if (shinyness) {
		pal = pal_table_shiny[species].pal_off;
	} else {
		pal = pal_table_nonshiny[species].pal_off;
	}
	
	u8 *img = gfx_table_pokemon_front[species].ptr_img;
	u16 img_size = 0x800;
	
	// insure pal tag will be a half word
	if (species > 0x199) {
		species = species - 0x199;
	}
	u8 id = create_oam(pal, img, img_size, x, y, species, cb);
	return id;
}

void oam_pkmn_back(u16 species, u8 shinyness, u16 x, u16 y, object_callback cb) {
	void *pal;
	
	// use shiny pal if shiny
	if (shinyness) {
		pal = pal_table_shiny[species].pal_off;
	} else {
		pal = pal_table_nonshiny[species].pal_off;
	}
	
	u8 *img = gfx_table_pokemon_back[species].ptr_img;
	u16 img_size = 0x800;
	
	// insure pal tag will be a halfword
	if (species > 0x199) {
		species = species - 0x199;
	}
	
	create_oam(pal, img, img_size, x, y, species, cb);
	return;
}

u8 oam_trainer_front(u16 img_id, u16 x, u16 y, object_callback cb) {
	img_id = ai_trainer[img_id].trainer_pic;
	void *pal = pal_table_trainer_front[img_id].pal_off;
	u8 *img = gfx_table_trainer_front[img_id].ptr_img;
	u16 img_size = 0x800;
	u8 id = create_oam(pal, img, img_size, x, y, img_id, cb);
	return id;
}

u8 oam_trainer_back(u16 id, u16 x, u16 y, object_callback cb) {

	// allocate palette
	u8 pal_id = gpu_pal_alloc_new((id *0x10) + 0x100);
	void *pal = pal_table_trainer_back[id].pal_off;
	pal_decompress_slice_to_faded_and_unfaded(pal, (0x100 + pal_id *0x10), 0x20);
	objt_pokemon[2].pal_tag = (id *0x10) + 0x100;	

	struct objtemplate backsprite_temp = uns_table_pokemon_trainer[id];
	objt_pokemon[2].tiles_tag = backsprite_temp.tiles_tag;
	objt_pokemon[2].oam = backsprite_temp.oam;
	objt_pokemon[2].graphics = backsprite_temp.graphics;
	objt_pokemon[2].rotscale = backsprite_temp.rotscale;
	objt_pokemon[2].callback = cb;
	objt_pokemon[2].animation = meta_animtable_trainer.trainer_anim[id];
	u8 obj_id = template_instanciate_forward_search(&objt_pokemon[2], x, y, 0x1E);
	objects[obj_id].x = x;
	objects[obj_id].y = y;
	return obj_id;
}

u8 hide_oam(u8 id) {
	objects[id].bitfield2 = 0x4;
	return (sizeof(objects[id]));
}

void display_oam(u8 id) {
	objects[id].bitfield2 = 0x0;
	return;
}

void del_oam(u8 id) {
	obj_delete_and_free_tiles(&objects[id]);
	return;
}

void oam_flip(u8 id, u8 dir) {
	// doesn't work yet!
	if (dir) {
		objects[id].bitfield = 0x2;
	} else {
		objects[id].bitfield = 0x1;
	}
	return;
}

u8 slide_trainer_player() {	
	u8 gender = saveblock2.sav2->gender;
	oam_trainer_back(gender, 0x50, 0x50, 0x0);
	return 0;
}

/*
void ball_throw(u8 task_id) {
	if (tasks[task_id].priv[3] > 0) {
		tasks[task_id].priv[3]--;
	} else {
		u8 ball_number_to_ball_processing_index(u8);
		void ball_to_ram_setup(u8);
		u8 type = tasks[task_id].priv[2];
		u8 index = ball_number_to_ball_processing_index(type);
		ball_to_ram_setup(index);
		struct objtemplate *test = &ball_templates[type];

		u8 id = template_instanciate_forward_search(test, 0, 0, 0x0);
		struct object *ball = &objects[id];
		ball->x = tasks[task_id].priv[0];
		ball->y = tasks[task_id].priv[1];
		
		objects[id].callback = (object_callback) 0x804B685;
		task_del(task_id);
		return;
	}
}*/


u8 ball_throw(u8 type, u16 x, u16 y) {
	u8 ball_number_to_ball_processing_index(u8);
	void ball_to_ram_setup(u8);
	
	u8 index = ball_number_to_ball_processing_index(type);
	ball_to_ram_setup(index);
	struct objtemplate *test = &ball_templates[type];
	test->callback = (object_callback) oac_nullsub_0;
	u8 id = template_instanciate_forward_search(test, x, y, 0x0);
	struct object *ball = &objects[id];
	ball->x = 32;
	ball->y = 64;
	objects[id].callback = (object_callback) 0x804B685;
	obj_del_delayed(id, 70);
	return 0;
}



