/* $Id: patchtkt.c,v 1.1.1.1 2001/03/25 17:15:28 root Exp $ */

#include <stdio.h>
#include <stdlib.h>

#include "patchtkt.h"

FILE *file_open(char *name, char *mode)
{
 FILE *rc;

 if((rc=fopen(name, mode))==NULL)
 {
  printf("Can't open %s\n", name);
  exit(1);
 }
 return(rc);
}

int verify(struct locator *l, FILE *stream, long offset)
{
 int i;
 unsigned char c;

 if(l->offset<0)
  return(1);
 fseek(stream, offset+l->offset, SEEK_SET);
 for(i=0; i<l->shift; i++)
 {
  if(fread(&c, 1, 1, stream)==0)
   break;
  if(c==l->c)
  {
   if(verify(l+1, stream, offset+i+l->offset))
    return(1);
   fseek(stream, offset+l->offset+i+1, SEEK_SET);
  }
 }
 return(0);
}

long locate(struct locator *l, FILE *stream, unsigned long first)
{
 char buf[512];
 int i, fetch;
 long cur_pos=first;

 fseek(stream, first, SEEK_SET);
 while((fetch=fread(buf, 1, sizeof(buf), stream))>0)
 {
  for(i=0; i<fetch; i++)
  {
   if(buf[i]==l->c&&verify(l+1, stream, cur_pos+i))
    return(cur_pos+i);
  }
  cur_pos+=fetch;
  fseek(stream, cur_pos, SEEK_SET);
 }
 return(-1);
}

void fcs(unsigned char c, FILE *stream)
{
 fwrite(&c, 1, 1, stream);
}

void fws(unsigned short w, FILE *stream)
{
 fwrite(&w, 2, 1, stream);
}
