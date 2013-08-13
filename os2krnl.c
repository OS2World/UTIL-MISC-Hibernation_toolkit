/* $Id: os2krnl.c,v 1.1.1.1 2001/03/25 17:15:28 root Exp $ */

#include "patchtkt.h"

/* IFS shutdown hook */

struct locator os2krnl_fsdhook[]=
{
 {0, 1, 0x8B}, 
 {1, 1, 0xF0},
 {1, 1, 0x68},
 {1, 1, 0x98},
 {1, 1, 0x00},
 {128, 512, 0x8B},
 {1, 1, 0x5E},
 {1, 1, 0xFC},
 {1, 1, 0x33},
 {1, 1, 0xC0},
 {1, 1, 0x3B},
 {1, 1, 0xC3},
 {1, 8, 0xC3},
 {-1, -1, 0} 
};

/* Patch area */

struct locator os2krnl_fsdpatch[]=
{
 {0, 1, 0x8B},
 {1, 1, 0x5E},
 {1, 1, 0xFC},
 {-1, -1, 0}
};

/* Device driver hook */

struct locator os2krnl_ddhook[]=
{
 {0, 1, 0x33},
 {1, 1, 0xF6},
 {1, 1, 0x8B},
 {1, 1, 0xFE},
 {1, 1, 0x8B},
 {1, 1, 0xCE},
 {8, 72, 0x8B},
 {1, 1, 0x4D},
 {1, 1, 0x08},
 {-1, -1, 0}
};

/* Patch area */

struct locator os2krnl_ddpatch[]=
{
 {0, 1, 0x8B},
 {1, 1, 0x4D},
 {1, 1, 0x08},
 {-1, -1, 0}
};

/* Version patch areas */

struct locator os2krnl_vp1[]=
{
 {0, 1, 0x00},
 {1, 1, 0x00},
 {1, 1, 0x00},
 {1, 1, 0x00},
 {1, 1, 20},
 {1, 1, 40},
 {1, 1, 0x00},
 {1, 1, 0x00},
 {-1, -1, 0}
};

struct locator os2krnl_pa1[]=
{
 {0, 1, 20},
 {1, 1, 40},
 {-1, -1, 0}
};

struct locator os2krnl_vp2[]=
{
 {0, 1, 'U'},
 {1, 1, 'N'},
 {1, 1, 'L'},
 {1, 1, 'A'},
 {1, 1, 'B'},
 {1, 1, 'E'},
 {1, 1, 'L'},
 {1, 1, 'E'},
 {1, 1, 'D'},
 {32, 32, 40},
 {1, 1, 20},
 {-1, -1, 0}
};

struct locator os2krnl_pa2[]=
{
 {0, 1, 40},
 {1, 1, 20},
 {-1, -1, 0}
};

struct locator os2krnl_vp3[]=
{
 {0, 1, 0x66},
 {1, 1, 0xC7},
 {1, 1, 0x43},
 {1, 1, 0x1C},
 {1, 1, 0x00},
 {1, 1, 0x00},
 {1, 1, 0x66},
 {1, 1, 0xC7},
 {1, 1, 0x43},
 {1, 1, 0x10},
 {1, 1, 40},
 {1, 1, 20},
 {-1, -1, 0}
};

struct locator os2krnl_pa3[]=
{
 {1, 1, 40},
 {1, 1, 20},
 {-1, -1, 0}
};

struct locator os2krnl_vp4[]=
{
 {1, 1, 0x80},
 {1, 1, 0x4B},
 {1, 1, 0x02},
 {1, 1, 0x10},
 {1, 1, 0xB8},
 {1, 1, 40},
 {1, 1, 0x80},
 {1, 1, 0x00},
 {1, 1, 0x00},
 {-1, -1, 0}
};

struct locator os2krnl_pa4[]=
{
 {1, 1, 40},
 {1, 1, 0x80},
 {-1, -1, 0}
};

/* Flip the version number */

static void flip_version(FILE *stream, unsigned long offset)
{
 char p;

 printf("Patching version level @ 0x%08lx\n", offset);
 fseek(stream, offset, SEEK_SET);
 while((p=fgetc(stream))!=40)
  if(p==EOF)
   return;
 fseek(stream, -1L, SEEK_CUR);
 fc(30);
}

/* Patch the version number */

int patch_os2krnl_level(FILE *stream)
{
 long o, patch_offset;
 int i;

 if((o=locate(os2krnl_vp1, stream, 0))<0||
    (patch_offset=locate(os2krnl_pa1, stream, o))<0)
 {
  printf("Patch area #1 not found\n");
  return(1);
 }
 flip_version(stream, patch_offset);
 if((o=locate(os2krnl_vp2, stream, patch_offset))<0||
    (patch_offset=locate(os2krnl_pa2, stream, o))<0)
 {
  printf("Patch area #2 not found\n");
  return(1);
 }
 flip_version(stream, patch_offset);
 for(i=0;
     (o=locate(os2krnl_vp3, stream, patch_offset))>=0&&
     (patch_offset=locate(os2krnl_pa3, stream, o))>=0;
     i++)
 {
  flip_version(stream, patch_offset);
 }
 if(i==0)
 {
  printf("Patch area #3 not found\n");
  return(1);
 }
 if(i==1)
  printf("Warning: only 1 occurence of patch area #3\n");
 if((o=locate(os2krnl_vp4, stream, patch_offset))<0||
    (patch_offset=locate(os2krnl_pa4, stream, o))<0)
 {
  printf("Patch area #4 not found\n");
  return(1);
 }
 flip_version(stream, patch_offset);
 return(0);
}

/* Patch the hibernation routines */

int patch_os2krnl_hib(FILE *stream)
{
 long fsdpatch_offset, ddpatch_offset;
 long o;

 if((o=locate(os2krnl_fsdhook, stream, 0))<0||
    (fsdpatch_offset=locate(os2krnl_fsdpatch, stream, o))<0)
 {
  printf("Can't locate the patch area in FlushFsdBuf routine!\n");
  return(1);
 }
 if((o=locate(os2krnl_ddhook, stream, 0))<0||
    (ddpatch_offset=locate(os2krnl_ddpatch, stream, o))<0)
 {
  printf("Can't locate the _NotifyDDSaveRestore->f_w_DDSaveRestore call!\n");
  return(1);
 }
 printf("Patching FlushFsdBuf @ 0x%08lx\n", fsdpatch_offset);
 fseek(stream, fsdpatch_offset, SEEK_SET);
 fc(0xBB); fw(0x0000);
 printf("Patching _NotifyDDSaveRestore @ 0x%08lx\n", ddpatch_offset);
 fseek(stream, ddpatch_offset, SEEK_SET);
 fc(0xEB); fc(0x08);
 return(0);
}
