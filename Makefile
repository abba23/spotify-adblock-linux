CC = gcc
# TODO do we need libyaml? = yaml-0.1 in pkg-config
CFLAGS = -Wall -I. $(shell pkg-config --cflags glib-2.0 libcyaml)
LDLIBS = -ldl -lcef -lglib-2.0 -lyaml -lcyaml $(shell pkg-config --libs glib-2.0 libcyaml)
# we must link against libcef.so to fix "undefined symbol: cef_string_userfree_utf16_free"
# libcef.so is provided by spotify (usually in /share/spotify)

TARGET = spotify-adblock
pname = spotify-adblock-linux

prefix ?= /usr/local
sysconfdir ?= /etc
bindir ?= /bin
libdir ?= /lib

CDEFINES = -DSYSCONFDIR=\"$(sysconfdir)\"

.PHONY: all
all: $(TARGET).so

$(TARGET).so: $(TARGET).c whitelist.h blacklist.h config.h
	$(CC) $(CFLAGS) $(CDEFINES) -shared -fPIC -o $@ $(LDFLAGS) $(LDLIBS) $^

.PHONY: clean
clean:
	rm -f $(TARGET).so

.PHONY: install
install:
	install -d $(DESTDIR)$(prefix)$(libdir)
	install -t $(DESTDIR)$(prefix)$(libdir) --strip -m 644 $(TARGET).so
	install -d $(DESTDIR)$(sysconfdir)/$(pname)
	install -t $(DESTDIR)$(sysconfdir)/$(pname) -m 644 config.yaml

.PHONY: uninstall
uninstall:
	rm -f $(prefix)/lib/$(TARGET).so
	rm -fr $(sysconfdir)/$(pname)
