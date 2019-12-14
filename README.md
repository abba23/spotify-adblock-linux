# spotify-adblock-linux
Spotify adblocker for Linux that works by wrapping `getaddrinfo` and *libcurl*'s `curl_easy_setopt` and blocking all domains that aren't whitelisted, as well as blacklisted URLs.

### Notes
* This does not work with the snap Spotify package.

## Building
Prerequisites:
* Make
* gcc
* libcurl headers (e.g. `libcurl4-gnutls-dev` on Debian/Ubuntu systems)

---

    $ git clone https://github.com/abba23/spotify-adblock-linux.git
    $ cd spotify-adblock-linux
    $ make

## Installing
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

## Uninstalling
    $ sudo make uninstall
