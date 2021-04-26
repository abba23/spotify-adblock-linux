#!/bin/sh
flatpak install -y flathub com.spotify.Client
wget -qO "$(pwd)/spotify-adblock.so" "https://github.com/abba23/spotify-adblock-linux/releases/download/$(curl -w '%{redirect_url}' -so /dev/null https://github.com/abba23/spotify-adblock-linux/releases/latest | rev | cut -d/ -f1 | rev)/spotify-adblock.so"
chmod +x "$(pwd)/spotify-adblock.so"
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
