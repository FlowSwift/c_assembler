CC=gcc
CFLAGS= -pedantic -Wall -g
TESTFLAGS= -pedantic -Wall -g
INCDIR=./include
INCLUDE=-I$(INCDIR)
SRCDIR=./src
OBJDIR=./obj
BINDIR=./bin

prod: main.c lines.o macros.o preprocessor.o util.o $(BINDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) main.c $(OBJDIR)/lines.o $(OBJDIR)/macros.o $(OBJDIR)/preprocessor.o $(OBJDIR)/util.o $(OBJDIR)/first_pass.o $(OBJDIR)/line_parser.o -o $(BINDIR)/test.test

test: main.c lines.o macros.o preprocessor.o util.o first_pass.o line_parser.o $(BINDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) main.c $(OBJDIR)/lines.o $(OBJDIR)/macros.o $(OBJDIR)/preprocessor.o $(OBJDIR)/util.o -o $(BINDIR)/test.test

lines.o: $(SRCDIR)/lines.c $(INCDIR)/lines.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/lines.c -o $(OBJDIR)/lines.o

macros.o: $(SRCDIR)/macros.c $(INCDIR)/macros.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/macros.c -o $(OBJDIR)/macros.o

preprocessor.o: $(SRCDIR)/preprocessor.c $(INCDIR)/preprocessor.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/preprocessor.c -o $(OBJDIR)/preprocessor.o

util.o: $(SRCDIR)/util.c $(INCDIR)/util.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/util.c -o $(OBJDIR)/util.o

first_pass.o: $(SRCDIR)/first_pass.c $(INCDIR)/first_pass.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/first_pass.c -o $(OBJDIR)/first_pass.o

line_parser.o: $(SRCDIR)/line_parser.c $(INCDIR)/line_parser.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/line_parser.c -o $(OBJDIR)/line_parser.o

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)