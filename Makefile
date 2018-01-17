
CFLAGS=-g -O2 -Wall -Wextra -Wno-implicit-fallthrough
LDFLAGS=
LOADLIBES=
PROGFILES=blak

WT=../wt/wt.sh
WT_WORK_DIR=./wt_work.tmp

all:$(PROGFILES)

#par_op.c:par_op.wt
#	$(WT) -d$(WT_WORK_DIR) -s c $<

#blak.h:par_op.wt
#	$(WT) -d$(WT_WORK_DIR) -s c $<

blak_OFILES=blak.o par_op.o

%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $<

blak:$(blak_OFILES)
	$(CC) $(LDFLAGS) -o $@ $^ $(LOADLIBES)

clean:
	rm -f $(PROGFILES) *.o Makefile.bak core *.core core.* 
	rm -rf $(WT_WORK_DIR)	
	
depend:
	makedepend -Y *.c 2> /dev/null
	
TAGS:
	ctags *

# DO NOT DELETE THIS LINE (generated dependencies start here)

blak.o: blak.h sysdefs.h
par_op.o: sysdefs.h
