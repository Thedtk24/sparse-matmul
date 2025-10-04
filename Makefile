CC = gcc-15
CFLAGS = -O2 -fopenmp -Wall -Wextra
SRC = src/main.c src/csr.c src/matmul_seq.c src/matmul_par.c
OBJ = $(SRC:.c=.o)
BIN = bin/matmul

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

re: clean all

.PHONY: all clean re
