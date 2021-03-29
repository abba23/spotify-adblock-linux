CC=gcc
CFLAGS=-Wall -I.
LDLIBS=-ldl
TARGET=spotify-adblock
PREFIX=/usr/local
CEF_URL=https://cef-builds.spotifycdn.com
# SAFE URL ENCODED and DECODED ("%2B" = "+")
CEF_URL_FILE=cef_binary_89.0.15%2Bgdef70e4%2Bchromium-89.0.4389.90_linux64_minimal.tar.bz2
CEF_FILE=cef_binary_89.0.15+gdef70e4+chromium-89.0.4389.90_linux64_minimal.tar.bz2

# Dummy file to keep track of if we have extracted archive or not
EXTRACT_CEF := $(addprefix .extract_,$(CEF_FILE))

.PHONY: all
all: $(TARGET).so

$(TARGET).so: $(TARGET).c whitelist.h blacklist.h | $(EXTRACT_CEF)
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $(LDFLAGS) $(LDLIBS) $^

.PHONY: clean
clean:
	rm -f $(TARGET).so
	rm -f $(CEF_FILE) $(CEF_FILE).sha1 $(EXTRACT_CEF)
	rm -rf include/

# Download CEF build and sha1sum of file
$(CEF_FILE):
	wget -N $(CEF_URL)/$(CEF_URL_FILE)
	wget -N $(CEF_URL)/$(CEF_URL_FILE).sha1 && echo "  $(CEF_FILE)" >> $(CEF_FILE).sha1

# Verify sha1sum of downloaded files and only on success continue with extract and creating dummy file
$(EXTRACT_CEF): .extract_%: %
	sha1sum -c $<.sha1 && tar -xf $< --wildcards '*/include' --strip-components=1 && touch $@

.PHONY: install
install: $(TARGET).so
	install --strip -Dm644 -t $(DESTDIR)$(PREFIX)/lib $^

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/lib/$(TARGET).so
