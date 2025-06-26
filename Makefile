BIN = pwinfo
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,%.o,$(SRC))
LIBS = $(wildcard *.a)
CC = gcc
CFLAGS = -I/usr/include/hidapi
LDFLAGS = -lhidapi-libusb

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

# static requires to build static libusb without libudev
static: $(OBJ)
	$(CC) $(CFLAGS) -static -o $(BIN) $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(BIN) $(OBJ) $(LIBS)

rebuild: clean all

.PHONY: all static clean rebuild
