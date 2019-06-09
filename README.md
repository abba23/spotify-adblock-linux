# spotify-adblock-linux
Spotify adblocker for Linux that works by wrapping `getaddrinfo` and `connect` and blocking all domains that aren't whitelisted, as well as blacklisted IP addresses.

*Note: It currently breaks some functionality (mostly by blocking connections to `35.186.224.53`, which is also used to deliver ads) like creating radio stations and the concerts tab*

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
