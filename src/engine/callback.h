#ifndef ENGINE_CALLBACK_H
#define ENGINE_CALLBACK_H

#include "../types.h"

typedef void (*task_callback)(u8);
typedef void (*super_callback)(void);

struct task {
	task_callback function;
	bool inuse;
	u8 prev;
	u8 next;
	u8 priority;
	u16 priv[16];
};

u8 task_add(task_callback func, u8 priority);
void task_del(u8 id);
void task_exec(void);

void set_callback2(super_callback func);
void set_callback1(super_callback func);

void vblank_handler_set(super_callback func);
void hblank_handler_set(super_callback func);

void c2_exit_to_overworld_1_continue_scripts_and_music(void);
void callback_clear_and_init(void);
void mem_cpy(void *dst, void *src, u32 size);

extern struct task tasks[16];

#endif /* ENGINE_CALLBACK_H */
