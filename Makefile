CC=gcc
CFLAGS=-Wall -shared -fPIC
LDLIBS=-ldl
TARGET=spotify-adblock
PREFIX=/usr/local/bin

all: $(TARGET).so

$(TARGET).so: $(TARGET).c whitelist.h blacklist.h
	$(CC) $(CFLAGS) -o $@ $(LDLIBS) $^

.PHONY: clean
clean:
	rm -f $(TARGET).so

.PHONY: install
install: $(TARGET).so
	cp $^ $(PREFIX)

.PHONY: install
uninstall:
	rm -f $(PREFIX)/$(TARGET).so
