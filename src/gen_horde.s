.align
.thumb

gen_horde_hook:			@0x0806CBE4 via r2
	mov r1, #0x5		@# pokes arg
	bl is_it_battle_time_3
	ldr r1, =(0x0806CBEC + 1)
	bx r1
