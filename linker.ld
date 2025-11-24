OUTPUT_ARCH(aarch64)
ENTRY(_start)

SECTIONS
{
	. = 0x80000000;
	__kernel_start = .;

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

	.page_tables (NOLOAD) : ALIGN(0x1000)
	{
		__page_tables_start = .;
		. = . + 3 * 0x1000;
		__page_tables_end = .;
		__page_tables_size = __page_tables_end - __page_tables_start;
	}

	.stack (NOLOAD) : ALIGN(0x1000){
		_stack_bottom = .;
		. += 0x10000;
			_stack_top = .;
	}
	__kernel_end = .;
}