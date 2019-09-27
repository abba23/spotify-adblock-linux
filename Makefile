CC=gcc
CFLAGS=-Wall -shared -fPIC
LDLIBS=-ldl
TARGET=spotify-adblock
PREFIX=/usr/local

.PHONY: all
all: $(TARGET).so

$(TARGET).so: $(TARGET).c whitelist.h blacklist.h
	$(CC) $(CFLAGS) -o $@ $(LDLIBS) $^

.PHONY: clean
clean:
	rm -f $(TARGET).so

.PHONY: install
install: $(TARGET).so
	install -Dm644 -t $(DESTDIR)$(PREFIX)/lib $^

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/lib/$(TARGET).so
