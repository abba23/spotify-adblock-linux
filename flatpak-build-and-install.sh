#!/bin/sh
wget -O cef.tar.bz2 https://cef-builds.spotifycdn.com/cef_binary_88.1.6%2Bg4fe33a1%2Bchromium-88.0.4324.96_linux64_minimal.tar.bz2
tar -xf cef.tar.bz2 --wildcards '*/include' --strip-components=1
flatpak install -y flathub org.freedesktop.Sdk//20.08
flatpak install -y flathub com.spotify.Client
flatpak run --filesystem="$(pwd):rw" --command=bash org.freedesktop.Sdk//20.08 <<EOF
make clean
make
EOF
. ./.flatpak-desktop.sh
