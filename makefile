CC=gcc
CFLAGS= -pedantic -Wall -g -ansi
TESTFLAGS= -pedantic -Wall -g -ansi
INCDIR=./include
INCLUDE=-I$(INCDIR)
SRCDIR=./src
OBJDIR=./obj
BINDIR=./bin

prod: assembler.c lines.o macros.o preprocessor.o util.o first_pass.o command_handler.o directive_handler.o line_parser.o error_handler.o symbol_table.o binary_conversion.o second_pass.o $(BINDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) assembler.c $(OBJDIR)/lines.o $(OBJDIR)/macros.o $(OBJDIR)/preprocessor.o $(OBJDIR)/util.o $(OBJDIR)/first_pass.o $(OBJDIR)/directive_handler.o $(OBJDIR)/command_handler.o $(OBJDIR)/line_parser.o $(OBJDIR)/error_handler.o $(OBJDIR)/symbol_table.o $(OBJDIR)/binary_conversion.o $(OBJDIR)/second_pass.o -o $(BINDIR)/assembler

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

command_handler.o: $(SRCDIR)/command_handler.c $(INCDIR)/command_handler.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/command_handler.c -o $(OBJDIR)/command_handler.o

directive_handler.o: $(SRCDIR)/directive_handler.c $(INCDIR)/directive_handler.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/directive_handler.c -o $(OBJDIR)/directive_handler.o

line_parser.o: $(SRCDIR)/line_parser.c $(INCDIR)/line_parser.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/line_parser.c -o $(OBJDIR)/line_parser.o

error_handler.o: $(SRCDIR)/error_handler.c $(INCDIR)/error_handler.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/error_handler.c -o $(OBJDIR)/error_handler.o

symbol_table.o: $(SRCDIR)/symbol_table.c $(INCDIR)/symbol_table.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/symbol_table.c -o $(OBJDIR)/symbol_table.o

binary_conversion.o: $(SRCDIR)/binary_conversion.c $(INCDIR)/binary_conversion.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/binary_conversion.c -o $(OBJDIR)/binary_conversion.o

second_pass.o: $(SRCDIR)/second_pass.c $(INCDIR)/second_pass.h $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(SRCDIR)/second_pass.c -o $(OBJDIR)/second_pass.o

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)