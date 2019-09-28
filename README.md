# spotify-adblock-linux
Spotify adblocker for Linux that works by wrapping `getaddrinfo` and *libcurl*'s `curl_easy_setopt` and blocking all domains that aren't whitelisted, as well as blacklisted URLs.

## Building
    $ git clone https://github.com/abba23/spotify-adblock-linux.git
    $ cd spotify-adblock-linux
    $ make

## Installing
    $ sudo make install

## Usage
    $ LD_PRELOAD=/usr/local/lib/spotify-adblock.so spotify

## Uninstalling
    $ sudo make uninstall

## Desktop file
```
[Desktop Entry]
Type=Application
Name=Spotify (adblock)
GenericName=Music Player
Icon=spotify-client
TryExec=spotify
Exec=env LD_PRELOAD=/usr/local/lib/spotify-adblock.so spotify %U
Terminal=false
MimeType=x-scheme-handler/spotify;
Categories=Audio;Music;Player;AudioVideo;
StartupWMClass=spotify
```
