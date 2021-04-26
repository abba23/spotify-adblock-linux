wget -O flatpak-spotify.sh https://raw.githubusercontent.com/flathub/com.spotify.Client/master/spotify-bin
sed -i flatpak-spotify.sh -e 's/LD_PRELOAD=/LD_PRELOAD='"$(pwd | sed 's/\//\\\//g')"'\/spotify-adblock.so:/g'
chmod +x flatpak-spotify.sh
flatpak override --user --filesystem="$(pwd):ro"
mkdir -p "$HOME/.local/share/applications"
cat > "$HOME/.local/share/applications/com.spotify.Client.desktop" <<EOF
[Desktop Entry]
Type=Application
Name=Spotify
GenericName=Online music streaming service
Comment=Access all of your favorite music
Icon=com.spotify.Client
Exec=flatpak run --command="$(pwd)/flatpak-spotify.sh" --file-forwarding com.spotify.Client @@u %U @@
Terminal=false
MimeType=x-scheme-handler/spotify;
Categories=Audio;Music;AudioVideo;
Keywords=Music;Player;Streaming;Online;
StartupWMClass=Spotify
X-GNOME-UsesNotifications=true
X-Flatpak-Tags=proprietary;
X-Flatpak=com.spotify.Client
EOF
