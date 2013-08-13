/* $Id: pmviop.c,v 1.1.1.1 2001/03/25 17:15:28 root Exp $ */

#include "patchtkt.h"

/* Patch for nonexistant ordinal to allow downgrade to 9.029 */

struct locator pmviop_ordinal[]=
{
 {0, 1, 0x6E},
 {1, 1, 0x03},
 {1, 1, 0x33},
 {1, 1, 0x01},
 {1, 1, 0x02},
 {1, 1, 0x01},
 {1, 1, 0xAE},
 {1, 1, 0x0C},
 {1, 1, 0x60},
 {1, 1, 0x0C},
 {1, 1, 0x7D},
 {-1, -1, 0} 
};

/* Patch the PMVIOP ordinal number */

int patch_pmviop_ordinal(FILE *stream)
{
 long patch_offset;
 int i;

 if((patch_offset=locate(pmviop_ordinal, stream, 0))<0)
 {
  printf("Patch area not found\n");
  return(1);
 }
 fseek(stream, patch_offset, SEEK_SET);
 fc(0x41);
 printf("Patching PMVIOP ordinal @ 0x%08lx\n", patch_offset);
 return(0);
}
