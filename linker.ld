OUTPUT_ARCH(aarch64)
ENTRY(_start)

SECTIONS
{
	. = 0x80000000;

	.text : ALIGN(0x1000) {
		*(.text)
	}

	.rodata : ALIGN(0x1000) {
		*(.rodata)
	}

	.data : ALIGN(0x1000) {
		*(.data)
	}

	.bss (NOLOAD) : ALIGN(0x1000) {
		__bss_start = .;
		*(.bss)
		*(COMMON)
		__bss_end = .;
	}

	.stack (NOLOAD) : ALIGN(0x1000){
		_stack_bottom = .;
		. += 0x10000;
			_stack_top = .;
	}
}