# spotify-adblock-linux
Spotify adblocker for Linux that works by wrapping `getaddrinfo` and `cef_urlrequest_create`, blocking non-whitelisted domains and blacklisted URLs.

### Notes
* This does not work with the snap and Flatpak Spotify packages.

## Build
Prerequisites:
* Git
* Wget
* tar
* Make
* GCC
* libc headers (e.g. `libc-dev` on Debian-based distributions)

---

    $ git clone https://github.com/abba23/spotify-adblock-linux.git
    $ cd spotify-adblock-linux
    $ wget -O cef.tar.bz2 https://cef-builds.spotifycdn.com/cef_binary_88.1.6%2Bg4fe33a1%2Bchromium-88.0.4324.96_linux64_minimal.tar.bz2
    $ tar -xf cef.tar.bz2 --wildcards '*/include' --strip-components=1
    $ make

## Install
    $ sudo make install

## Usage

### Command-line
    $ LD_PRELOAD=/usr/local/lib/spotify-adblock.so spotify

### Desktop file
You can also integrate it with your desktop environment by creating a `.desktop` file (e.g. `spotify-adblock.desktop`) in `~/.local/share/applications`. This lets you easily run it from an application launcher without opening a terminal.

<details> 
  <summary>Example</summary>
  <p>

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
  </p>
</details>

## Uninstall
    $ sudo make uninstall
