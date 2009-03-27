/* hfload.c
 * 
 * This file is subject to the terms and conditions of the GNU
 * General Public License.  See the file "COPYING" in the main
 * directory of this archive for more details.
 *
 * Copyright (C) 2000, Jay Carlson
 */

/* 
 * Boot loader main program.
 */

#include <unistd.h>
#include <linux/elf.h>
#include "hfload.h"

int file_offset;
int old_stack_pointer;
                
#define MAX_PHDRS_SIZE 8

Elf32_Ehdr header;
Elf32_Phdr phdrs[MAX_PHDRS_SIZE];

extern  void flush_cache(void);

void 
zero_region(char *start, char *end)
{
        char *addr;
        int count;

        count = end - start;

#ifndef CONFIG_NO_PRINTK    
	printf("zeroing from %08x to to %08x, 0x%x bytes\n", start, end, count);
#endif

#ifndef FAKE_COPYING
        memset(start, 0, count);
#endif
}        

void 
load_phdr(Elf32_Phdr *phdr) {
        char *addr, *end;

        seek_forward(phdr->p_offset);

        addr = (char *)phdr->p_vaddr;
        end = ((char *)addr) + phdr->p_memsz;

	copy_to_region(addr, phdr->p_filesz);
        
        addr = ((char *)addr) + phdr->p_filesz;

        zero_region(addr, end);
}

void main(unsigned long stack_start_addr)
{
        int i;
        file_offset = 0;
#ifdef CONFIG_SERIAL_CONSOLE
	display_on();
        printf("entering boot loader, turning on display\n");
#endif

#ifndef CONFIG_NO_PRINTK
        printf("decompressing kernel:\n");
#endif

	decompress_kernel(UNCOMPRESS_OUT, stack_start_addr+4096,FREEMEM_END, 0);
	
#ifndef CONFIG_NO_PRINTK
        printf("done decompressing kernel.\n");
#endif   

#if 0	
		init_read();
        read_struct(&header, sizeof(header));
#ifndef CONFIG_NO_PRINTK
        printf("e_entry: %08x, e_ehsize: %d, e_phentsize %d, e_phnum %d,\n",
               header.e_entry, header.e_ehsize, header.e_phentsize, header.e_phnum);

        printf("e_shentsize %d, e_shnum %d\n", header.e_shentsize, header.e_shnum);
#endif        
        if (header.e_phnum > MAX_PHDRS_SIZE) {
#ifndef CONFIG_NO_PRINTK                
	printf("maximum phdr count %d, found %d; exiting\n", MAX_PHDRS_SIZE, header.e_phnum);
#endif                
			exit(1);
        }

        if (header.e_phentsize != sizeof(Elf32_Phdr)) {
#ifndef CONFIG_NO_PRINTK
	printf("phentsize != sizeof(Elf32_Phdr). Exiting.\n");
#endif
				exit(1);
        }

        seek_forward(header.e_phoff);

        for (i = 0; i < header.e_phnum; i++) {
                read_struct(phdrs + i, sizeof(Elf32_Phdr));
        }

        for (i = 0; i < header.e_phnum; i++) {
                if (phdrs[i].p_type == PT_LOAD) {
                        load_phdr(phdrs + i);
                }
        }
#ifndef CONFIG_NO_PRINTK
        printf("done loading kernel, about to jump kernel=%08x!\n",header.e_entry);
#endif	
#endif	
		start_kernel(0x80000000);
}
