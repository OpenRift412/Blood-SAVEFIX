
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

CPPFLAGS = /4r /s /orti /oe=3 /dAPPVER_EXEDEF=$(appver_exedef)
CFLAGS =
AFLAGS = -ml


.h: $(INC_DIR)
.cpp: $(SRC_DIR)
.c: $(SRC_DIR)
.asm: $(SRC_DIR)
.obj: $(appver_exedef)\$(OBJ_DIR)

OBJS = &
qheap.obj &
resource.obj &
iob.obj &
crc32.obj &
trig.obj &
getopt.obj &
error.obj &
debug4g.obj &
textio.obj &
pqueue.obj &
key.obj &
inifile.obj &
mouse.obj &
misc.obj &
gfx.obj

all : $(appver_exedef)\qtools.lib .symbolic
    @%null

$(appver_exedef)\qtools.lib: $(OBJS)
    %make $(appver_exedef)\qtools.lnk
	cd $(appver_exedef)
	wlib qtools.lib /n /b @qtools.lnk
	cd ..

$(appver_exedef)\qtools.lnk: makefile
    %create $^@
    for %i in ($(OBJS)) do %append $^@ -+$(OBJ_DIR)\%i


.cpp.obj:
    wpp386 $(CPPFLAGS) $[* /fo=$(appver_exedef)\$(OBJ_DIR)\$^&

.asm.obj:
    tasm $(AFLAGS) $[*.asm $(appver_exedef)\$(OBJ_DIR)\$^&