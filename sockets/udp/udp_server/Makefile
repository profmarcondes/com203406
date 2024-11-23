
CC=gcc
CFLAGS=
SRC = src/main.c

EXE = udp_server


$(EXE): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: all
all: $(EXE)

.PHONY: clean
clean:
	rm -f $(EXE)
