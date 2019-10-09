CC = musl-gcc
LD = $(CC)
CFLAGS = -Wextra -Wall -g -Os
LDFLAGS = -s -static


OBJ = bf2nasm.o
BIN = bf2nasm

all: $(BIN)

$(BIN): $(OBJ)
		$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

clean:
	rm -f $(BIN) $(OBJ)

.PHONY:
	all clean
