CC=gcc
CFLAGS=-Wall -shared -fPIC
LDLIBS=-ldl
TARGET=spotify-adblock

all: $(TARGET).so

$(TARGET).so: $(TARGET).c whitelist.h
	$(CC) $(CFLAGS) -o $@ $(LDLIBS) $^
