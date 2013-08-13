/* $Id: patchtkt.h,v 1.1.1.1 2001/03/25 17:15:28 root Exp $ */

#include <stdio.h>

/* Search structure */

struct locator
{
 long offset;
 long shift;
 unsigned char c;
};

FILE *file_open(char *name, char *mode);
int verify(struct locator *l, FILE *stream, long offset);
long locate(struct locator *l, FILE *stream, unsigned long first);
void fcs(unsigned char c, FILE *stream);
void fws(unsigned short w, FILE *stream);

#define fc(c) fcs(c, stream)
#define fw(w) fws(w, stream)
