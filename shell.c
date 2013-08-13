/* $Id: shell.c,v 1.1 2001/03/31 12:54:02 root Exp $ */

#include "patchtkt.h"

/* Patch to avoid hangs when returning from dedicated sessions (forces
   cold boot) */

struct locator shell_return[]=
{
 {0, 1, 0xCD},
 {1, 1, 0x19},
 {1, 1, 0xB8},
 {1, 1, 0x00},
 {1, 1, 0x4C},
 {1, 1, 0xCD},
 {1, 1, 0x21},
 {-1, -1, 0} 
};

/* Patch the return code for SHELL.COM */

int patch_shell_return(FILE *stream)
{
 long patch_offset;
 int i;

 if((patch_offset=locate(shell_return, stream, 0))<0)
 {
  printf("Patch area not found\n");
  return(1);
 }
 fseek(stream, patch_offset, SEEK_SET);
 fc(0xEA);
 fw(0x0000);
 fw(0xFFFF);
 printf("Patching SHELL.COM return sequence @ 0x%08lx\n", patch_offset);
 return(0);
}
