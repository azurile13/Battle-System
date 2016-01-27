/*
 * Data
 */


vblank_cb_battle_BG0HOFS = 0x02022974;
vblank_cb_battle_BG0VOFS = 0x02022976;
vblank_cb_battle_BG1HOFS = 0x02022978;
vblank_cb_battle_BG1VOFS = 0x0202297A;
vblank_cb_battle_BG2HOFS = 0x0202297C;
vblank_cb_battle_BG2VOFS = 0x0202297E;
vblank_cb_battle_BG3HOFS = 0x02022980;
vblank_cb_battle_BG3VOFS = 0x02022982;
vblank_cb_battle_WIN0H = 0x02022984;
vblank_cb_battle_WIN0V = 0x02022986;
vblank_cb_battle_WIN1H = 0x02022988;
vblank_cb_battle_WIN1V = 0x0202298A;
dp12_8700 = 0x02038700;
dp12_config = 0x02039600;
script_env_1 = 0x03000EB0;
script_env_2 = 0x03000F28;
pal_fade_control = 0x02037AB8;
map_post_load_hook = 0x03005020;
tasks = 0x03005090;
objects = 0x0202063C;
poke_oam_battle = 0x0824F018;
anim_poke = 0x082349BC;
rotscale_empty = 0x08231CFC;
oac_nullsub = 0x0810BB89;
obj_ids_to_display = 0x02021800;
evolution_table = 0x08259754;
pokemon_bank = 0x0202402C;
base_stats = 0x08254784;
dex_data = 0x0844E850;
battle_env_table = 0x0824EE34;
temp_vars = 0x020370B8;
battle_data_ptrs = 0x0203C000;
battle_load_global = 0x0203F000; /*Free RAM space*/
superstate = 0x030030F0;
displayed_string = 0x02021D18;
textflags = 0x03003E50;


/*
* Place holder data
*/

berries_t = 0x8740000;
sheer_force_t = 0x8740000;
mega_items_t = 0x8740000;
megas = 0x8740000;
type_chart = 0x8740000;
battle_strings = 0x8740000;


/*
 * Functions
 */

rboxid_clear_pixels = 0x0800445C|1;
rboxid_08003FA0 =  0x08003FA0|1;
rboxid_to_vram = 0x08003F20|1;
something_08050DE1 = 0x08050DE0|1;
gpu_tile_obj_alloc_tag_and_upload = 0x080086DC|1;
gpu_tile_obj_alloc_tag_and_apply = 0x08008928|1;
gpu_sync_bg_show = 0x080019BC|1;
something_08050969 = 0x08050968|1;
battle_show_message_maybe = 0x080D87BC|1;
box_print = 0x0812E51C|1;
sub_0800F380 = 0x0800F380|1;
sub_0800F34C = 0x0800F34C|1;



c2_exit_to_overworld_1_continue_scripts_and_music = 0x080568C4|1;

script_env_2_enable = 0x08069940|1;
overworld_free_bgmaps = 0x080563F0|1;
lcd_io_set = 0x08000A38|1;
lcd_io_get = 0x08000AC4|1;
vblank_handler_set = 0x080006F4|1;
hblank_handler_set = 0x08000700|1;
gpu_tile_bg_drop_all_sets = 0x08001618|1;
bg_positions_reset = 0x0812D594|1;
callback_clear_and_init = 0x080F4E24|1;
bgid_nullify_tilemap = 0x08001FD4|1;
mapldr_continue_scripts_restart_music = 0x0807DD24|1;
c2_exit_to_overworld_2_switch = 0x080567DC|1;
gpu_sprites_upload = 0x08007320|1;
copy_queue_process = 0x08007610|1;
gpu_pal_upload = 0x08070474|1;
bg_scroll_invert = 0x08087F54|1;
dp12_reset_something = 0x08087EA4|1;
dp12_something = 0x08087EE4|1;
dp12_update = 0x08087F54|1;
dp12_b_callback = 0x08087FE0|1;
dp12_abort = 0x08087E64|1;
pal_decompress_slice_to_faded_and_unfaded = 0x080703A8|1;
get_bs_elem_env_index = 0x0800FC2C|1;

load_var_value = 0x0806E568|1;
var_set_value = 0x0806E584|1;
var_get_pointer = 0x0806E454|1;
memcpy = 0x081E5E78|1;
malloc = 0x08002B9C|1;
malloc_and_clear = 0x08002BB0|1;
free = 0x08002BC4|1;
task_add = 0x0807741C|1;
task_del = 0x08077508|1;
set_callback2 = 0x08000544|1;
set_callback1 = 0x080565E0|1;
fade_screen = 0x08070588|1;
sp198_help_system_disable = 0x0812B478|1;
malloc_and_LZ77UnComp = 0x080F6AA0|1;
gpu_copy_to_tileset = 0x080017D0|1;
gpu_copy_tilemap = 0x08002040|1;
bgid_send_tilemap = 0x080020BC|1;
gpu_pal_apply = 0x080703EC|1;
lz77_uncomp_vram = 0x081E3B6C|1;

task_exec = 0x08077578|1;
textbox_something = 0x08002DE8|1;
objc_exec = 0x08006B5C|1;
obj_sync_something = 0x08006BA8|1;
fade_and_return_progress_probably = 0x080704D0|1;
decompress_with_fallback_maybe = 0x080F6878|1;
gpu_sync_bg_show = 0x080019BC|1;
gpu_sync_bg_hide = 0x080019D0|1;

bgid_set_tilemap = 0x08001FA0|1;
bg_vram_setup = 0x08001658|1;
memset = 0x081E5ED8|1;

template_instanciate_forward_search = 0x08006F8C|1;
obj_delete = 0x080073DC|1;

get_attribute = 0x0803FBE8|1;
set_attribute = 0x0804037C|1;
divmod_remainder = 0x081E4684|1;
rand = 0x08044EC8|1;
__aeabi_idiv = 0x081E4684|1;
__aeabi_uidiv = 0x081E4684|1;

species_to_dex_index = 0x08043298|1;

strcpy_xFF_terminated = 0x08008D84|1;
sub_080F6EE4 = 0x080F6EE4|1;
box_related_one = 0x080F6CD0|1;
sav2_get_text_speed = 0x080F78A8|1;
rboxid_to_vram = 0x08003F20|1;
sub_81220D4 = 0x081220D4|1;
box_curved = 0x080F7768|1;
sub_080F696C = 0x080F696C|1;
sub_08002040 = 0x08002040|1;
gpu_sync_bg_show = 0x80019BC|1;

/*
 * IO
 */

BG3HOFS = 0x0400001C;
DMA0_SRC = 0x040000B0;
DMA0_DST = 0x040000B4;
DMA0_CNT_L = 0x040000B8;
DMA0_CNT_H = 0x040000BA;

/*
 * Custom Data
 */

interpreter_state = 0x02036E30;
