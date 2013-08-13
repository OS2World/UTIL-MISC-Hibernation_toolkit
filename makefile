# $Id: makefile,v 1.2 2001/03/31 12:54:01 root Exp $

PROJ = hibpatch

CC = icc.exe
LINKER = link386.exe
!ifdef DEBUG
DEBUG_COPT = /Fl+ /Ti+
DEBUG_LINKOPT = /DE
!endif
!ifdef LIBC
ADD_COPT = /I$(LIBCPATH) /Re
!else
ADD_COPT = /Se /Gi+
!endif
!ifdef EXP
ENABLE_EXP = /DENABLE_EXP=1
!endif
COPT = /c /G4 $(DEBUG_COPT) $(ADD_COPT) $(ENABLE_EXP) /Gs- /Fo$@ %s
!ifdef LIBC
LINKLIB = LIBCS+OS2386
ADD_LINKOPT = /NOD
!else
LINKLIB =
!endif
LINKOPT = $(DEBUG_LINKOPT) $(ADD_LINKOPT) /PMTYPE:VIO
LINKDEF = $(PROJ).DEF

.SUFFIXES: .c .obj .exe

all: $(PROJ).exe
OBJS = $(PROJ).obj \
       patchtkt.obj os2krnl.obj os2ldr.obj pmviop.obj shell.obj
$(PROJ).exe: $(OBJS) packexe.exe
 $(LINKER) $(LINKOPT) $(OBJS),$(@B),NUL,$(LINKLIB),$(LINKDEF);
!if "$(DEBUG)" == ""
 LXLITE /CS /BDX- /I- /ZS $(PROJ).exe
 packexe $(PROJ).exe
!endif
packexe.exe: packexe.obj \
             patchtkt.obj
 $(LINKER) $(LINKOPT) $**,$(@B),NUL,$(LINKLIB),$(LINKDEF);
!if "$(DEBUG)" == ""
 LXLITE /CS /BDX- /I- packexe.exe
!endif

$(PROJ).obj: $(PROJ).c
 $(CC) $(COPT)
packexe.obj: packexe.c
 $(CC) $(COPT)
patchtkt.obj: patchtkt.c
 $(CC) $(COPT)
os2krnl.obj: os2krnl.c
 $(CC) $(COPT)
os2ldr.obj: os2ldr.c
 $(CC) $(COPT)
pmviop.obj: pmviop.c
 $(CC) $(COPT)
shell.obj: shell.c
 $(CC) $(COPT)
