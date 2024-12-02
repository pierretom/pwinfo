BIN = pwinfo
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,%.o,$(SRC))
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
	$(CC) $(CFLAGS) -static -o $(BIN) $^ $(LDFLAGS) libusb-1.0.a

clean:
	rm -f $(BIN) $(OBJ)

rebuild: clean all

.PHONY: all clean rebuild
