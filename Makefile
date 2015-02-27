CC=gcc
CFLAGS=-Wall -Werror
SOURCES=syntax.c symtab.c filebuf.c errmsg.c lexer.c parser.c
OBJECTS=$(SOURCES:.c=.o)

# Targets
all: $(OBJECTS)
	$(CC) $(CLFAGS) $(OBJECTS) -o syntax

$(OBJECTS):
	$(CC) $(CFLAGS) -c $(@:.o=.c)

scanner: $(OBJECTS)
	$(CC) $(CFLAGS) -c scanner.c
	$(CC) $(CFLAGS) symtab.o filebuf.o lexer.o scanner.o -o scanner
clean:
	rm -f $(OBJECTS) $(OBJECTS:.o=) scanner.o scanner syntax
