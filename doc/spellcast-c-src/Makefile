# These three lines define the fonts that spellcast tries to use by default;
# it looks for FONT1 first, then FONT2, then FONT3. (If a user sets
# his spellcast*font resource, that is tried first, instead of FONT1.)
# You can change these to suit your environment. 
FONT1 = \"-*-times-medium-r-*--14-*\"
FONT2 = \"-*-helvetica-medium-r-*--12-*\"
FONT3 = \"fixed\"

# The next line defines the directory in which game transcripts are
# stored. It should be a fully-qualified pathname, beginning (but
# not ending) with a slash.
LOGDIR = \"/tmp\"

# The next lines define the directories where the executable, man page, and
# spell list postscript file are installed. Change them to whatever is appropriate.
DESTDIR=/afs/andrew.cmu.edu/usr0/games
DESTBIN=$(DESTDIR)/bin/.unpriv/@sys
DESTMAN=$(DESTDIR)/man/man6
DESTLIB=$(DESTDIR)/lib/spellcast

# If you are compiling on a Sun and you find the game crashes
# all the time, try uncommenting the next line.
#OTHERLIBS = -lbsdmalloc

CFLAGS = -g -I/usr/local/include -I./bitmaps -DBODYFONT=$(FONT1) \
	-DBODYFONT_ALT=$(FONT2) -DBODYFONT_ALT2=$(FONT3) -DTRANSCRIPTDIR=$(LOGDIR)
LFLAGS = -g

XLIB = -L/usr/local/lib -lX11

OBJS = engine.o espells.o execute.o execute2.o emonster.o etran.o
XOBJS = main.o xtext.o xquery.o xloop.o xgest.o xbutton.o xtalk.o

all: spellcast

# spellcast: the X executable
spellcast: $(OBJS) $(XOBJS)
	$(CC) $(LFLAGS) -o spellcast $(OBJS) $(XOBJS) $(XLIB) $(OTHERLIBS)

# stupid: the stupid stdin/stdout version of the game
stupid: stupid.o $(OBJS)
	$(CC) $(LFLAGS) -o spellcast stupid.o $(OBJS)

# makelist: a small program used to create spelllist.ps, which you
# shouldn't have to use, because spelllist.ps is already there.
makelist: makelist.o espells.o
	$(CC) $(LFLAGS) -o makelist makelist.o espells.o

install: spellcast
	install -c -s spellcast $(DESTBIN)
	install -c spellcast.6 $(DESTMAN)
	-mkdir $(DESTLIB)
	install -c spelllist.ps $(DESTLIB)

main.o: handwave.h spelllist.h xspell.h patchlevel.h
xtext.o: handwave.h spelllist.h xspell.h
xquery.o: handwave.h spelllist.h xspell.h
xloop.o: handwave.h spelllist.h xspell.h
xgest.o: handwave.h spelllist.h xspell.h
xbutton.o: handwave.h spelllist.h xspell.h
xtalk.o: handwave.h spelllist.h xspell.h

internal.h: spelllist.h

engine.o: handwave.h spelllist.h internal.h
espells.o: handwave.h spelllist.h internal.h
execute.o: handwave.h spelllist.h internal.h
execute2.o: handwave.h spelllist.h internal.h
emonster.o: handwave.h spelllist.h internal.h
etran.o: handwave.h spelllist.h internal.h

stupid.o: stupid.c handwave.h spelllist.h

# The following rule creates the spelllist.ps document,
# but you shouldn't have to use it, because it's already
# there.
spelllist.ps: makelist bitmaps/base.ps
	./makelist > tmp.ps
	cat bitmaps/base.ps tmp.ps > spelllist.ps

clean:
	rm -f *~ *.o spellcast stupid makelist tmp.ps core
