/* $Id: packexe.c,v 1.1.1.1 2001/03/25 17:15:28 root Exp $ */

#include "patchtkt.h"

/* Patch the packed executable */

int pack_exe(FILE *stream, char *msg)
{
 char lx[2];
 long patch_offset;
 int i;

 fseek(stream, 0L, SEEK_SET);
 if(fread(lx, 1, 2, stream)<2||memcmp(lx, "LX", 2))
 {
  printf("Not an unstubbed LX executable, can't pack\n");
  return(1);
 }
 fseek(stream, 0L, SEEK_END);
 patch_offset=ftell(stream);
 if(patch_offset>32870)
 {
  printf("The EXE file is too large\n");
  return(1);
 }
 fseek(stream, 8L, SEEK_SET);
 fc(0xEB); fc(0x7F);
 fseek(stream, 0x89L, SEEK_SET);
 fc(0xE9); fw(patch_offset-0x8C);
 fseek(stream, 0L, SEEK_END);
 fc(0xB4); fc(0x09);
 fc(0xBA); fw(patch_offset+0x109);
 fc(0xCD); fc(0x21);
 fc(0xCD); fc(0x20);
 fprintf(stream, "%s$", msg);
 return(0);
}

/* Main routine */

void main(int argc, char **argv)
{
 FILE *stream;

 if(argc<2)
 {
  printf("Usage: %s <filename.ext>\n", argv[0]);
  exit(1);
 }
 stream=file_open(argv[1], "r+b");
 pack_exe(stream, "OS/2 v 3.0 or later required\r\n");
 fclose(stream);
}
