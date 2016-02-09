#include "types.h"
#include "ROM_tables.h"
#include "battle_engine_resource.h"

char string_buffer[1000];
char buffer1[16];
char buffer2[32];
char buffer3[20];
char buffer4[20];

char *str_cpy(char *, char *);
char *fmt_int_10(char *, char *, u8, u8);

//static u8 t_name_size = 0xD;


void flush_string_buffer() {
	void mem_set(void *, u32, u32);
	mem_set(string_buffer, 0xFF, 1000);
	return;
}

char *str_cpy_len(char *dst, char *src, u32 size) {
	while (size > 0) {
		*dst++ = *src++;
	}
	return dst;
}

u32 string_length(char *string) {
	char this_char = (char) 0;
	u32 size = 0;
	do {
		this_char = *string++;
		size++;
	} while (this_char != (char)0xFF);
	return size;
}

char *int_to_str(char *dst, char *src) {
	return fmt_int_10(dst, src, 1, 3);
}

char *gen_string(char c) {
	switch (c) {
		// Player name
		case 0:
			return (saveblock2.sav2->name);
			
		// Normal use of FD buffers
		case 1:
			return str_cpy(buffer1, buffer1);
		case 2:
			return str_cpy(buffer2, buffer2);
		case 3:
			return str_cpy(buffer3, buffer3);
		case 4:
			return str_cpy(buffer4, buffer4);
		case 6:
			// opponent1 name trainer
			return (ai_trainer[battle_mallocd_resources.battle_field->trainer_flag[0]].trainer_name);
		case 7:
			// opponent1 name trainer
			return (ai_trainer[battle_mallocd_resources.battle_field->trainer_flag[1]].trainer_name);
		case 8:
			// opponent1 name trainer
			return (ai_trainer[battle_mallocd_resources.battle_field->trainer_flag[2]].trainer_name);
		case 9:
			// opponent1 name trainer
			return (ai_trainer[battle_mallocd_resources.battle_field->trainer_flag[3]].trainer_name);
		case 0xA:
			// opponent1 name trainer
			return (ai_trainer[battle_mallocd_resources.battle_field->trainer_flag[4]].trainer_name);
		case 0xB:
			// opponent name pkmn1
			return (pkmn_names_table[battle_mallocd_resources.battle_field->trainer_flag[0]].name);
		case 0xC:
			// opponent name pkmn2
			return (pkmn_names_table[battle_mallocd_resources.battle_field->trainer_flag[1]].name);
		case 0xD:
			// opponent name pkmn3
			return (pkmn_names_table[battle_mallocd_resources.battle_field->trainer_flag[2]].name);
		case 0xE:
			// opponent name pkmn4
			return (pkmn_names_table[battle_mallocd_resources.battle_field->trainer_flag[3]].name);
		case 0xF:
			// opponent name pkmn5
			return (pkmn_names_table[battle_mallocd_resources.battle_field->trainer_flag[4]].name);
		case 0x10:
			// move name
			return (move_names[battle_mallocd_resources.battle_field->multi_use].name);
		case 0x11:
			// ability name
			return (ability_names[battle_mallocd_resources.battle_field->multi_use].name);
		case 0x12:
			// type name
			return (typenames[battle_mallocd_resources.battle_field->multi_use].typename);
		case 0x13:
			// item name
			{
			u16 item_index = battle_mallocd_resources.battle_field->multi_use;
			return (items[item_index].name);
			}
		case 0x14:
			// Buffered number 
			return int_to_str(buffer1, buffer2);
		case 0x15:
			// trainer class
			{
			u8 id = battle_mallocd_resources.battle_field->trainer_flag[0];
			id = ai_trainer[id].trainer_class;
			//return trainer_class_names[id * 13];
			return(trainer_class_names[id].name);
			break;
			}
		default:			
			break;
			
	};
	return ((char *)0x08415A20); // empty string
}


void decoder(char *string) {
	flush_string_buffer();
	u32 strlen = string_length(string);
	char *string_buff_p = string_buffer;
	u8 i;
	for (i = 0; i < strlen; i++) {
		char c = *(string + i);
		switch (c) {
			case 0xFD:
				{
				// fd byte, call inplace string swap
				char *to_cpy = gen_string(string[i + 1]);
				str_cpy(string_buff_p, to_cpy);
				string_buff_p += string_length(to_cpy) - 1;
				i++;
				break;
				}
			case 0xFF:
				return;
			default:
				// non special character
				*string_buff_p = (char) c;
				string_buff_p++;
				break;
		};
	}
	return;
}