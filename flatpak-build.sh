#!/bin/sh
flatpak install flathub org.freedesktop.Sdk//20.08
flatpak install flathub com.spotify.Client
flatpak run --filesystem="$(pwd):rw" --command=bash org.freedesktop.Sdk//20.08 <<EOF
make clean
make
EOF
flatpak override --user --filesystem="$(pwd):ro" --env=LD_PRELOAD="$(pwd)/spotify-adblock.so" com.spotify.Client
