CC=gcc
CFLAGS=-Wall
LDLIBS=-ldl
TARGET=spotify-adblock
PREFIX=/usr/local

.PHONY: all
all: $(TARGET).so

$(TARGET).so: $(TARGET).c whitelist.h blacklist.h
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $(LDLIBS) $^

.PHONY: clean
clean:
	rm -f $(TARGET).so

.PHONY: install
install: $(TARGET).desktop $(TARGET).so
	install -Dm644 -t $(DESTDIR)$(PREFIX)/share/applications $(TARGET).desktop
	install -Dm644 -t $(DESTDIR)$(PREFIX)/lib $(TARGET).so

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/share/applications/$(TARGET).desktop $(PREFIX)/lib/$(TARGET).so
