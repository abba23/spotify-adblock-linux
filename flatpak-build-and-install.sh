#!/bin/sh
wget -O cef.tar.bz2 https://cef-builds.spotifycdn.com/cef_binary_88.1.6%2Bg4fe33a1%2Bchromium-88.0.4324.96_linux64_minimal.tar.bz2
tar -xf cef.tar.bz2 --wildcards '*/include' --strip-components=1
flatpak install -y flathub org.freedesktop.Sdk//20.08
flatpak install -y flathub com.spotify.Client
flatpak run --filesystem="$(pwd):rw" --command=bash org.freedesktop.Sdk//20.08 <<EOF
make clean
make
EOF
flatpak override --user --filesystem="$(pwd):ro"
mkdir -p "$HOME/.local/share/applications"
cat > "$HOME/.local/share/applications/com.spotify.Client.desktop" <<EOF
[Desktop Entry]
Type=Application
Name=Spotify
GenericName=Online music streaming service
Comment=Access all of your favorite music
Icon=com.spotify.Client
Exec=flatpak run --command=/app/extra/bin/spotify --env=LD_PRELOAD="$(pwd)/spotify-adblock.so" --file-forwarding com.spotify.Client @@u %U @@
Terminal=false
MimeType=x-scheme-handler/spotify;
Categories=Audio;Music;AudioVideo;
Keywords=Music;Player;Streaming;Online;
StartupWMClass=Spotify
X-GNOME-UsesNotifications=true
X-Flatpak-Tags=proprietary;
X-Flatpak=com.spotify.Client
EOF
