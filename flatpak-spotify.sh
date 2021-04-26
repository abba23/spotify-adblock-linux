#!/bin/sh

SCALE_FACTOR=`get-scale-factor.py`

function openuri {
    # Spotify doesn't properly support single instance or even just opening URIs itself so lets hack around it:
    # If a URI is passed we will try to activate an existing instance over MPRIS.
    for arg in "$@"; do
    if [ "${arg:0:8}" = "spotify:" ]; then
        echo "Attempting to open $arg in existing instance..."
        if gdbus call -e -d org.mpris.MediaPlayer2.spotify -o /org/mpris/MediaPlayer2 -m org.mpris.MediaPlayer2.Player.OpenUri "$arg" > /dev/null 2> /dev/null; then
            return 0
        else
            return 1
        fi
    fi
    done
    return 2
}

function waitandopenuri {
    if ! gdbus wait -e -t 5 org.mpris.MediaPlayer2.spotify; then
        echo "Failed to open URI, MPRIS never appeared."
    else
        openuri "$@"
    fi
}

openuri "$@"
URI_HANDLED=$?
# If URI_HANDLED = 0, We raised an existing instance.
# If URI_HANDLED = 1, We found a URI but no existing instance.
#                     In this case we make an instance and then open the URI again...
# If URI_HANDLED = 2, No URI.
# (Forgive terrible shell usage)
if [ $URI_HANDLED -eq 0 ]; then
    exit 0
fi

env PULSE_PROP_application.icon_name="com.spotify.Client" LD_PRELOAD=/home/user/Documents/code/github/rany2/spotify-adblock-linux/spotify-adblock.so:/app/lib/spotifywm.so /app/extra/bin/spotify --force-device-scale-factor=$SCALE_FACTOR "$@" &
`set-dark-theme-variant.py` &

if [ $URI_HANDLED -eq 1 ]; then
    waitandopenuri "$@"
fi
