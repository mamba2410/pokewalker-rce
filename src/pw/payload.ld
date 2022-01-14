

__payload_min_start_addr	= 0xf956;
__payload_max_end_addr		= 0xff40;

__pw_ram_start		= 0xf780;
__pw_ram_end		= 0xff80;

/* addr here is big-endian */
__func_ptr_global		= 0xf7e0;
__func_normal_main_loop	= 0x08d6;

__payload_start_addr = __payload_min_start_addr;

SECTIONS {

	. = __payload_start_addr;

	__payload_text_start = .;
	.text : { *(.text .text.*) }
	__payload_text_end = .;

	__payload_rodata_start = .;
	.rodata : { *(.rodata) }
	__payload_rodata_end = .;

	__payload_data_start = .;
	.data : { *(.data) }
	__payload_data_end = .;

	__payload_bss_start = .;
	.bss : { *(.bss) }
	__payload_bss_end = .;

	
}
