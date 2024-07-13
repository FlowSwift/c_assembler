CC=gcc
CFLAGS=-ansi -pedantic -Wall -g
TESTFLAGS=-pedantic -Wall -g
INCDIR=./include
INCLUDE=-I$(INCDIR)
SRCDIR=./src
OBJDIR=./obj
BINDIR=./bin

test: main.c lines.o macros.o preprocessor.o util.o $(BINDIR)
	$(CC) $(TESTFLAGS) $(INCLUDE) main.c $(OBJDIR)/lines.o $(OBJDIR)/macros.o $(OBJDIR)/preprocessor.o $(OBJDIR)/util.o -o $(BINDIR)/test.test

lines.o: $(SRCDIR)/lines.c $(INCDIR)/lines.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/lines.c -o $(OBJDIR)/lines.o

macros.o: $(SRCDIR)/macros.c $(INCDIR)/macros.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/macros.c -o $(OBJDIR)/macros.o

preprocessor.o: $(SRCDIR)/preprocessor.c $(INCDIR)/preprocessor.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/preprocessor.c -o $(OBJDIR)/preprocessor.o

util.o: $(SRCDIR)/util.c $(INCDIR)/util.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/util.c -o $(OBJDIR)/util.o

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)