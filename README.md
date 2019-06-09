# spotify-adblock-linux
Spotify adblocker for Linux that works by wrapping `getaddrinfo` and blocking all connections that aren't whitelisted.

*Note: It currently breaks some functionality (mostly by blocking connections to `spclient.wg.spotify.com`, which is also used to deliver ads) like the friends activity feed, radio, recently played list and concerts tab.*

## Building
    $ git clone https://github.com/abba23/spotify-adblock-linux.git
    $ cd spotify-adblock-linux
    $ make

## Installing
    $ sudo make install

## Usage
    $ LD_PRELOAD=/usr/local/bin/spotify-adblock.so spotify

## Uninstalling
    $ sudo make uninstall
