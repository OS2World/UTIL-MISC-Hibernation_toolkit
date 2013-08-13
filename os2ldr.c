/* $Id: os2ldr.c,v 1.1.1.1 2001/03/25 17:15:28 root Exp $ */

#include "patchtkt.h"

#ifdef ENABLE_EXP

/* The HYBERLDR filespec */

struct locator os2ldr_hyberldr[]=
{
 {0, 1, 'H'}, 
 {1, 1, 'Y'},
 {1, 1, 'B'},
 {1, 1, 'E'},
 {1, 1, 'R'},
 {1, 1, 'L'},
 {1, 1, 'D'},
 {1, 1, 'R'},
 {1, 1, 0},
 {-1, -1, 0} 
};

/* Hibernation entries mapping routine. Guess offset values from there. */

struct locator os2ldr_hibmap[]=
{
 {0, 1, 0x55}, 
 {1, 1, 0x8B},
 {1, 1, 0xEC},
 {1, 1, 0x56},
 {1, 1, 0x57},
 {1, 1, 0x8B},
 {1, 1, 0x5E},
 {1, 1, 0x04},
 {1, 1, 0x8B},
 {1, 1, 0x46},
 {1, 1, 0x06},
 {24, 32, 0xF7},
 {1, 1, 0xF1},
 {28, 16, 0xC7},
 {28, 16, 0x04},
 {-1, -1, 0} 
};

/* Hibernation procedure offsets (in hibmap) */

struct locator os2ldr_hibmap_offsets[]=
{
 {0, 1, 0xC7},
 {1, 1, 0x04},
 {9, 1, 0xC7},
 {1, 1, 0x04},
 {9, 1, 0xC7},
 {1, 1, 0x04},
 {-1, -1, 0} 
};

/* Slack area where the stub should be inserted */

struct locator os2ldr_slack[]=
{
 {0, 1, 0xC8},
 {1, 1, 0x10},
 {1, 1, 0x00},
 {1, 1, 0x00},
 {1, 1, 0xC7},
 {1, 1, 0x46},
 {1, 1, 0xF6},
 {7, 5, 0xC7}, 
 {1, 1, 0x46},
 {1, 1, 0xF8},                          /* Sub-entry */
 {64, 80, 0xF6},
 {1, 1, 0x07},
 {1, 1, 0x18},
 {-1, -1, 0}
};

/* Sub-entry to get offset where the hibernation-related data can be stored */

struct locator os2ldr_slack_subentry[]=
{
 {0, 1, 0xC7}, 
 {1, 1, 0x46},
 {1, 1, 0xF8},
 {-1, -1, 0} 
};

/* Yet another slack area */

struct locator os2ldr_slack2[]=
{
 {0, 1, 0xC8},
 {1, 1, 0x0A},
 {1, 1, 0x00},
 {1, 1, 0x00},
 {3, 5, 0xC4},
 {1, 1, 0x5E},
 {1, 1, 0x08},
 {6, 20, 0x0C},
 {1, 1, 0x20},
 {8, 24, 0x3A},
 {1, 1, 0xC1},
 {4, 16, 0xC3},
 {-1, -1, 0}
};

/* Offset of the SYS2029 output routine */

struct locator os2ldr_sys2029[]=
{
 {0, 1, 0xB8}, 
 {1, 1, 0xED},
 {1, 1, 0x07},
 {3, 16, 0xB8}, 
 {1, 1, 0x4A},
 {1, 1, 0x0C},
 {3, 16, 0xB8}, 
 {1, 1, 0x4B},
 {1, 1, 0x0C},
 {-1, -1, 0} 
};

/* Memory mover */

struct locator os2ldr_memmove[]=
{
 {0, 1, 0x55},
 {1, 1, 0x8B},
 {1, 1, 0xEC},
 {3, 5, 0xC5},
 {1, 1, 0x76},
 {1, 1, 0x04},
 {12, 24, 0xF3},
 {1, 1, 0x66},
 {1, 1, 0xA5},
 {4, 8, 0xF3},
 {1, 1, 0xA4},
 {4, 4, 0xC3},
 {-1, -1, 0}
};

/* Code area which references the hibernation procedure parameters */

struct locator os2ldr_ref1[]=
{
 {0, 1, 0x2E},
 {1, 1, 0x83},
 {1, 1, 0x3E},
 {3, 1, 0x00},
 {8, 20, 0xF8},
 {1, 1, 0x02},
 {1, 1, 0xB9},
 {1, 1, 0x04},
 {1, 1, 0x00},
 {1, 1, 0xFC},
 {1, 1, 0xF2},
 {1, 1, 0xAF},
 {-1, -1, 0}
};

/* Code which is subjected to patch */

struct locator os2ldr_ref2[]=
{
 {0, 1, 0xA1},
 {3, 1, 0x50},
 {1, 1, 0x0E},
 {1, 1, 0xE8},
 {3, 1, 0x83},
 {1, 1, 0xC4},
 {1, 1, 0x06},
 {1, 1, 0xE8},
 {3, 1, 0x83},
 {1, 1, 0x3E},
 {5, 16, 0xE8},                         /* <- insertion */
 {3, 4, 0xBA},
 {1, 1, 0xF2},
 {1, 1, 0x03},
 {-1, -1, 0}
};

/* Patch insertion point */

struct locator os2ldr_insertion[]=
{
 {0, 1, 0xE8},
 {3, 4, 0xBA},
 {1, 1, 0xF2},
 {1, 1, 0x03},
 {-1, -1, 0}
};

/* Patch insertion point #2 (note: we get rid of mode 12h completely!) */

struct locator os2ldr_insertion2[]=
{
 {0, 1, 0xB4},
 {1, 1, 0x00},
 {1, 1, 0xB0},
 {1, 1, 0x12},
 {1, 1, 0xCD},
 {1, 1, 0x10},
 {1, 1, 0xB4},
 {1, 1, 0x00},
 {1, 1, 0xB0},
 {1, 1, 0x03},
 {1, 1, 0xCD},
 {1, 1, 0x10},
 {-1, -1, 0}
};

int patch_os2ldr(FILE *stream)
{
 long patch_offset, patch2_offset, hyberldr_offset, insertion_offset;
 long insertion2_offset;
 long o, o2;
 int i;
 unsigned short tmem_base, hibentry_base, sys2029_base, memmove_base;
 unsigned short ref1_base, ref2_base, a20flick_base, hdc_base, posthib_base;
 unsigned short posthib_proc_base, hibdata_base;

 fseek(stream, 0L, SEEK_END);
 if(ftell(stream)>=57344)
 {
  printf("Loader is too large!\n");
  return(1);
 }
 if((hyberldr_offset=locate(os2ldr_hyberldr, stream, 0))<0)
 {
  printf("Can't find reference the HYBERLDR file!\n");
  return(1);
 }
 if((o=locate(os2ldr_hibmap, stream, 0))<0)
 {
  printf("Can't find the hibernation mapper routine!\n");
  return(1);
 }
 if((o=locate(os2ldr_hibmap_offsets, stream, o))<0)
 {
  printf("Can't find offset table!\n");
  return(1);
 }
 fseek(stream, o+8, SEEK_SET);
 fread(&hibentry_base, 2, 1, stream);
 if((o=locate(os2ldr_sys2029, stream, 0))<0)
 {
  printf("Can't find SYS2029 output routine\n");
  return(1);
 }
 sys2029_base=o;
 if((o=locate(os2ldr_memmove, stream, 0))<0)
 {
  printf("Can't find the memory mover routine\n");
  return(1);
 }
 memmove_base=o;
 if((o=locate(os2ldr_ref1, stream, 0))<0||
    (o2=locate(os2ldr_ref2, stream, 0))<0)
 {
  printf("Can't find reference to hibernation memory variables\n");
  return(1);
 }
 fseek(stream, o+3, SEEK_SET);
 fread(&ref1_base, 2, 1, stream);
 fseek(stream, o2+1, SEEK_SET);
 fread(&ref2_base, 2, 1, stream);
 if((insertion_offset=locate(os2ldr_insertion, stream, o2))<0||
    (insertion2_offset=locate(os2ldr_insertion2, stream, o2))<0||
    (patch_offset=locate(os2ldr_slack, stream, 0))<0||
    (patch2_offset=locate(os2ldr_slack2, stream, 0))<0)
 {
  printf("No place for the patch code!\n");
  return(1);
 }
 printf("Code at 0x%lx will be routed to 0x%lx and 0x%lx\n",
        insertion_offset, patch_offset, patch2_offset);
 if((o=locate(os2ldr_slack_subentry, stream, patch_offset))<0)
  return(1);
 fseek(stream, o+3, SEEK_SET);
 fread(&tmem_base, 2, 1, stream);
 tmem_base+=0x268;
 /* Point of no return */
 /* Generate the post-hibernation code */
 fseek(stream, patch_offset, SEEK_SET);
 fc(0xC8); fc(0x08); fw(0x0000);
 fc(0x56);
 fc(0x1E);
 fc(0x68); fw(tmem_base);
 fc(0x1E);
 fc(0x68); fw(hyberldr_offset);
 fc(0xFF); fc(0x1E); fw(hibentry_base);
 fc(0x83); fc(0xC4); fc(0x08);
 fc(0x0B); fc(0xC0);
 fc(0x74); fc(0x03);
 fc(0xE8); fw(sys2029_base-ftell(stream)-2);
 fc(0x83); fc(0x3E); fw(tmem_base+2); fc(0x00);
 fc(0x75); fc(0x08);
 fc(0x81); fc(0x3E); fw(tmem_base); fw(0x1000);
 fc(0x76); fc(0x03);
 fc(0xE8); fw(sys2029_base-ftell(stream)-2);
 fc(0xFF); fc(0x36); fw(tmem_base+2);
 fc(0xFF); fc(0x36); fw(tmem_base);
 fc(0x8B); fc(0x46); fc(0x06);
 fc(0x05); fc(0xFF); fc(0x0F);
 fc(0x25); fc(0x0F); fc(0xF0);
 fc(0xC1); fc(0xE8); fc(0x04);
 fc(0x8A); fc(0x6E); fc(0x05);
 fc(0x2A); fc(0xC9);
 fc(0x8B); fc(0xD0);
 fc(0x03); fc(0xC1);
 fc(0x80); fc(0xC4); fc(0x01);
 fc(0x2B); fc(0xC9);
 fc(0x89); fc(0x4E); fc(0xFC);
 fc(0x89); fc(0x46); fc(0xFE);
 fc(0x50);
 fc(0x51);
 fc(0x51);
 fc(0x51);
 fc(0x8B); fc(0xF2);
 fc(0xFF); fc(0x1E); fw(hibentry_base+4);
 fc(0x83); fc(0xC4); fc(0x0C);
 fc(0xFF); fc(0x1E); fw(hibentry_base+8);
 fc(0x8B); fc(0x46); fc(0x08);
 fc(0x8A); fc(0x6E); fc(0x05);
 fc(0x2A); fc(0xC9);
 fc(0x03); fc(0xF1);
 fc(0x2B); fc(0xDB);
 fc(0x8E); fc(0xC6);
 fc(0x89); fc(0x5E); fc(0xF8);
 fc(0x8C); fc(0x46); fc(0xFA);
 fc(0x26); fc(0x89); fc(0x87); fw(0x0088);
 fc(0x68); fw(0x1000);
 fc(0x26); fc(0x8A); fc(0x67); fc(0x1A);
 fc(0x2A); fc(0xC0);
 fc(0x2B); fc(0xC9);
 fc(0x50);
 fc(0x51);
 fc(0x06);
 fc(0x53);
 fc(0xE8); fw(memmove_base-ftell(stream)-2);
 fc(0x83); fc(0xC4); fc(0x0A);
 fc(0x68); fw(0x1000);
 fc(0xC4); fc(0x5E); fc(0xF8);
 fc(0x26); fc(0x8A); fc(0x67); fc(0x1C);
 fc(0x2A); fc(0xC0);
 fc(0x2B); fc(0xC9);
 fc(0x50);
 fc(0x51);
 fc(0xFF); fc(0x76); fc(0xFE);
 fc(0xFF); fc(0x76); fc(0xFC);
 fc(0xE8); fw(memmove_base-ftell(stream)-2);
 fc(0x83); fc(0xC4); fc(0x0A);
 fc(0x5E);
 fc(0xC9);
 fc(0xC3);
 /* Generate the hook */
 fseek(stream, insertion_offset+1, SEEK_SET);
 fread(&a20flick_base, 2, 1, stream);
 a20flick_base+=ftell(stream);
 fseek(stream, patch2_offset, SEEK_SET);
 fc(0xA1); fw(ref1_base);
 fc(0x50);
 fc(0xA1); fw(ref2_base);
 fc(0x50);
 fc(0x0E);
 fc(0xFA);
 fc(0xE8); fw(patch_offset-ftell(stream)-2);
 fc(0x83); fc(0xC4); fc(0x06);
 fc(0xE8); fw(a20flick_base-ftell(stream)-2);
 fc(0xC3);
 fseek(stream, insertion_offset, SEEK_SET);
 fc(0xE8); fw(patch2_offset-ftell(stream)-2);
 /* Append a HDC setup routine */
/* fseek(stream, 0L, SEEK_END);
 for(i=0; i<8000; i++)
  fc(0x00);
 hdc_base=ftell(stream); */
 hdc_base=0x26C9; fseek(stream, 0x26C9, SEEK_SET);  /* DH */
 fc(0xB8); fc(0xA0); fc(0x00);
 fc(0xBA); fc(0x76); fc(0x01);
 fc(0xEF);
 fc(0x66); fc(0x51);
 fc(0x66); fc(0xB9); fc(0xFF); fc(0xFF); fc(0x00); fc(0x00);
 fc(0xE2); fc(0xFE);
 fc(0x66); fc(0x59);
 fc(0xB8); fc(0x00); fc(0x00);
 fc(0xBA); fc(0x77); fc(0x01);
 fc(0xEF);
 fc(0x66); fc(0x51);
 fc(0x66); fc(0xB9); fc(0xFF); fc(0xFF); fc(0x00); fc(0x00);
 fc(0xE2); fc(0xFE);
 fc(0x66); fc(0x59);
 fc(0xB8); fc(0xB0); fc(0x00);
 fc(0xBA); fc(0x76); fc(0x01);
 fc(0xEF);
 fc(0x66); fc(0x51);
 fc(0x66); fc(0xB9); fc(0xFF); fc(0xFF); fc(0x00); fc(0x00);
 fc(0xE2); fc(0xFE);
 fc(0x66); fc(0x59);
 fc(0xB8); fc(0x00); fc(0x00);
 fc(0xBA); fc(0x77); fc(0x01);
 fc(0xEF);
 fc(0xC3);
 fseek(stream, insertion2_offset, SEEK_SET);
 fc(0xE8); fw(hdc_base-ftell(stream)-2);
 fc(0x90);
 fc(0x90);
 fc(0x90);
 posthib_base=0x7B2C; fseek(stream, 0x7B2C, SEEK_SET);  /* DH */
 fw(0x00); fw(0x00); fw(0x00); fw(0x00); /* QWORD */
 fc(0xEA);
 fw(0x00); fw(0x00); /* FAR JMP */
 fw(0x00);
 posthib_proc_base=ftell(stream);
 hibdata_base=tmem_base-0x352;
 fc(0x66); fc(0x33); fc(0xC0);
 fc(0xA1); fw(hibdata_base);
 fc(0x66); fc(0xC1); fc(0xE0); fc(0x0C);
 fc(0x66); fc(0x8B); fc(0xF8);
 fc(0x66); fc(0x33); fc(0xC0);
 fc(0xA1); fw(hibdata_base+2);
 fc(0x66); fc(0xC1); fc(0xE0); fc(0x0C);
 fc(0x66); fc(0x8B); fc(0xF0);
 fc(0x66); fc(0x33); fc(0xC0);
 fc(0x8C); fc(0xC8);
 fc(0x66); fc(0xC1); fc(0xE0); fc(0x04);
 fc(0x66); fc(0x05); fc(0x98); fc(0x8E); fw(0x0000); /* 0x8E98 --? */
 fc(0x66); fc(0xA3); fw(posthib_base+9);
 fc(0xA1); fw(hibdata_base+0x16);
 fc(0xA3); fw(posthib_base+13);
 fc(0xA1); fw(hibdata_base+0x0E);
 fc(0xC1); fc(0xE0); fc(0x08);
 fc(0x8E); fc(0xE8);
 fc(0x66); fc(0x33); fc(0xD2);
 fc(0x8B); fc(0x16); fw(hibdata_base+0x16);
 fc(0x66); fc(0xB8); fc(0xFF); fc(0xFF); fc(0x00); fc(0x00);
 fc(0x65); fc(0x67); fc(0x66); fc(0x89); fc(0x02);
 fc(0x66); fc(0xB8); fc(0x00); fc(0x9B); fc(0xCF); fc(0x00);
 fc(0x65); fc(0x67); fc(0x66); fc(0x89); fc(0x4); fc(0x204);
 fc(0x8B); fc(0x16); fw(hibdata_base+0x18);
 fc(0x66); fc(0xB8); fc(0xFF); fc(0xFF); fc(0x00); fc(0x00);
 fc(0x65); fc(0x67); fc(0x66); fc(0x89); fc(0x02);
 fc(0x66); fc(0xB8); fc(0x00); fc(0x93); fc(0xCF); fc(0x00);
 fc(0x65); fc(0x67); fc(0x66); fc(0x89); fc(0x42); fc(0x04);
 fc(0x66); fc(0x33); fc(0xC0);
 fc(0x8C); fc(0xE8);
 fc(0x66); fc(0xC1); fc(0xE0); fc(0x04);
 fc(0x66); fc(0xA3); fw(posthib_base+2);
 fc(0xA1); fw(hibdata_base+0x10);
 fc(0xA3); fw(posthib_base);
 fc(0xFA);
 fc(0x0F); fc(0x01); fc(0x16); fw(posthib_base);
 fc(0x0F); fc(0x01); fc(0xE0);
 fc(0x0D); fc(0x01); fc(0x00);
 fc(0x0F); fc(0x01); fc(0xF0);
 fc(0xEB); fc(0x01);
 fc(0x90);
 fc(0x8E); fc(0xDA);
 fc(0x8E); fc(0xC2);
 fc(0x8E); fc(0xD2);
 fc(0xE9); fw(posthib_base-ftell(stream)+6); /* JMP (not FAR) to FAR JMP */
 return(0);
}

#endif
