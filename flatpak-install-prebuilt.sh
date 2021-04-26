#!/bin/sh
flatpak install -y flathub com.spotify.Client
wget -O spotify-adblock.so "https://github.com/abba23/spotify-adblock-linux/releases/download/$(curl -w '%{redirect_url}' -so /dev/null https://github.com/abba23/spotify-adblock-linux/releases/latest | rev | cut -d/ -f1 | rev)/spotify-adblock.so"
. ./.flatpak-desktop.sh
