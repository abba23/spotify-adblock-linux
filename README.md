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
      
## Usage for fish shell
    $ env LD_PRELOAD=/usr/local/lib/spotify-adblock.so spotify
    
## Uninstalling
    $ sudo make uninstall
