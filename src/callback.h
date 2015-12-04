#ifndef CALLBACK_H
#define CALLBACK_H

typedef void (*callback)();

void set_callback2(callback cb);
void vblank_handler_set(callback cb);
void hblank_handler_set(callback cb);

void task_exec(void);
void objc_exec(void);
void obj_sync_something(void);

#endif
