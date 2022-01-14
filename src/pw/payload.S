/* vim: set ft=gas: */

.include "pw_funcs.h"

.section .text

.global payload_begin
payload_begin:
	jsr common_prologue
	jsr wdt_pet

	/* Open poke radar app */
	jsr open_pokeradar_app

	/* Return to normal OS */
	mov.w #normal_main_loop, r0
	jsr set_loop_func

	jsr common_epilogue
    rts

