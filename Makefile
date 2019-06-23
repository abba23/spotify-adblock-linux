CC=gcc
CFLAGS=-Wall -shared -fPIC
LDLIBS=-ldl
TARGET=spotify-adblock
PREFIX=/usr/local/lib

all: $(TARGET).so

$(TARGET).so: $(TARGET).c whitelist.h blacklist.h
	$(CC) $(CFLAGS) -o $@ $(LDLIBS) $^

.PHONY: clean
clean:
	rm -f $(TARGET).so

.PHONY: install
install: $(TARGET).so
	cp $^ $(PREFIX)

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/$(TARGET).so
