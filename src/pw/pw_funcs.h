/* vim: set ft=gas: */
.ifndef PW_FUNCS_H
.set PW_FUNCS_H, 1


/*
 *	Common functions
 */
.set normal_main_loop,		0x08d6
.set common_prologue,		0xba42
.set common_epilogue,		0xba62

.set wdt_pet,				0x259e

/*
 *	Control functions
 */
.set set_loop_func,			0x693a	/* aka setProcToCallByMainInLoop */


/*
 *	Gameplay functions
 */
.set open_pokeradar_app,	0x9d92	/* aka goToPokeRadarMicroApp */

/*
 *	Util functions
 */
.set memcpy,				0xb9ce	/* r0=src, r1=dst, r2=len */


.endif /* PW_FUNCS_H */
