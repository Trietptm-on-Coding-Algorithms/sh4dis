/* a shared object that can be called easily with python+ctypes */

/* */
#include <stdint.h>
#include <string.h>

/* c++ stuff */
#include <map>
#include <string>
#include <vector>
#include <iostream>

/* libopcodes stuff */
#define PACKAGE "bfd"
#define PACKAGE_VERSION "2.30"
#include <bfd.h> // for bfd_arch_arm, etc.
#include <dis-asm.h>

int cb_fprintf(void *stream, const char *fmt, ...)
{
	va_list args;
    va_start(args, fmt);
    char *built_str = (char *)stream;
	char buf[1024];
    int rc = vsnprintf(buf, sizeof(buf)-1, fmt, args);
    va_end(args);
    strcat((char *)built_str, buf);
    return rc;
}

extern "C" int get_disasm_libopcodes(uint32_t addr, uint8_t *data, int len, char *result)
{
	/* initialize disassembler, if needed */
	static bool loc_init = false;
	static disassemble_info dinfo = {0};
	static disassembler_ftype disasm;
	if(!loc_init) {
		/* create disassemble info */
		init_disassemble_info(&dinfo, NULL, cb_fprintf);
		dinfo.flavour = bfd_target_unknown_flavour;
		dinfo.arch = bfd_arch_sh;
		dinfo.mach = bfd_mach_sh4;
		dinfo.endian = BFD_ENDIAN_BIG;
		disassemble_init_for_target(&dinfo); // reads dinfo.arch and populate extra stuff

		/* create disassembler */
		disasm = disassembler(bfd_arch_sh, TRUE, bfd_mach_sh4, NULL);
		if(!disasm) {
			printf("ERROR: disassembler() returned no function\n");
			return -1;
		}

		loc_init = true;
	}

	/* use the stream pointer as our private data
		(the buffer that fprintf() should append to */
	dinfo.stream = (void *)result;

	/* call disassembler
		will use callbacks in dinfo (like .read_memory_func, .print_addr_func, etc.)
		and the defaults are fine for this use case, see the defaults in a debugger
		or look at especially buffer_read_memory() in dis-buf.c for details */
	dinfo.octets_per_byte = 1;
	dinfo.buffer_vma = addr;
	dinfo.stop_vma = addr+2;
	/* source data */
	dinfo.buffer = data;
	dinfo.buffer_length = len;

	result[0] = '\0';
	disasm((bfd_vma)addr, &dinfo);

	return 0;
}
