# spotify-adblock-linux
Spotify adblocker for Linux that works by wrapping `getaddrinfo` and `cef_urlrequest_create`, blocking non-whitelisted domains and blacklisted URLs.

### Notes
* This does not work with the snap Spotify packages.

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

## Install on Flatpak

Go to the folder with `spotify-adblock.so` (/usr, /bin, or /app aren't going to work), save the flatpak spotify wrapper, and grant the flatpak permission to access those files with `flatpak override`:  

```
$ flatpak run --command=bash com.spotify.Client -c 'cat "$(command -v spotify)"' > flatpak-spotify-adblock.sh
$ flatpak override --user --filesystem="$(pwd):ro"
```

Now you need to modify `flatpak-spotify-adblock.sh` to have `spotify-adblock.so` in its `LD_PRELOAD` list, you do this by finding `LD_PRELOAD` in `flatpak-spotify-adblock.sh` and adding `spotify-adblock.so` to it (each object is delimited by either a space or colon). For example, you'd replace this line:   

```
env PULSE_PROP_application.icon_name="com.spotify.Client" LD_PRELOAD=/app/lib/spotifywm.so /app/extra/bin/spotify --force-device-scale-factor=$SCALE_FACTOR "$@" &
```

with:   

```
env PULSE_PROP_application.icon_name="com.spotify.Client" LD_PRELOAD=/path/to/spotify-adblock.so:/app/lib/spotifywm.so /app/extra/bin/spotify --force-device-scale-factor=$SCALE_FACTOR "$@" &
```

You could also automate the above by running the following sed command: `sed -i flatpak-spotify-adblock.sh -e 's/LD_PRELOAD=/LD_PRELOAD='"$(pwd | sed 's/\//\\\//g')"'\/spotify-adblock.so:/g'`  

To run the Spotify flatpak app, you would use `flatpak run --command="/path/to/flatpak-spotify-adblock.sh" com.spotify.Client`.  

Replace `/path/to/flatpak-spotify-adblock.sh` with the path to that script.

## Usage

### Command-line
    $ LD_PRELOAD=/usr/local/lib/spotify-adblock.so spotify

### Desktop file
You can also integrate it with your desktop environment by creating a `.desktop` file (e.g. `spotify-adblock.desktop`) in `~/.local/share/applications`. This lets you easily run it from an application launcher without opening a terminal.  

For flatpak, the `Icon` would be `com.spotify.Client` and `Exec` line would be in the form `flatpak run --command="/path/to/flatpak-spotify-adblock.sh" --file-forwarding com.spotify.Client @@u %U @@`.

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
