CC=gcc
CFLAGS=-Wall
LDLIBS=-ldl
TARGET=spotify-adblock
PREFIX=/usr/local/lib

.PHONY: all
all: $(TARGET).so

$(TARGET).so: $(TARGET).c whitelist.h blacklist.h
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $(LDLIBS) $^

.PHONY: clean
clean:
	rm -f $(TARGET).so

.PHONY: install
install: $(TARGET).so
	cp $^ $(PREFIX)

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/$(TARGET).so
