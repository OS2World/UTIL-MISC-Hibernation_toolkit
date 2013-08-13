/* $Id: hibpatch.c,v 1.2 2001/03/31 12:54:01 root Exp $ */

#include <stdio.h>
#include <stdlib.h>

#include "patchtkt.h"
#include "os2ldr.h"
#include "os2krnl.h"
#include "pmviop.h"
#include "shell.h"

void main(int argc, char **argv)
{
 FILE *stream;

 printf("HIBPATCH v 1.10 on " __DATE__ ", " __TIME__ "\n\n");
 if(argc<3)
 {
  printf("Usage: %s <filename.ext> <patch type>\n"
         "\n"
         "Where: <filename.ext> is the name of file to patch\n"
         "       <patch type> is:\n"
         "             /KRNLFIX = fix hibernation problems in recent OS/2 v 4.0 kernels\n"
         "                        (XR_M006 and higher) and Aurora\n"
         "            /SHELLFIX = fix SHELL.COM to prevent hangs on return from\n"
         "                        dedicated DOS session (forces cold boot)\n"
         "               /WARP3 = bring an OS/2 v 4.0 kernel to Warp 3 level\n"
         "              /PMVIOP = stub away nonexistent imports in PMVIOP.DLL\n"
         "                        (to use backlevel OS/2 v 4.0 kernels with newer\n"
         "                        Warp 3 DLLs)\n",
         argv[0]);
  exit(1);
 }
 stream=file_open(argv[1], "r+b");
 if(!stricmp(argv[2], "/KRNLFIX"))
  patch_os2krnl_hib(stream);
 if(!stricmp(argv[2], "/SHELLFIX"))
  patch_shell_return(stream);
 else if(!stricmp(argv[2], "/WARP3"))
  patch_os2krnl_level(stream);
 else if(!stricmp(argv[2], "/PMVIOP"))
  patch_pmviop_ordinal(stream);
 else
  printf("Invalid option: %s\n", argv[2]);
 fclose(stream);
}
