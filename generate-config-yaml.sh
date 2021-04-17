#!/usr/bin/env bash

header2yaml() {
  cat $1 | grep '^    "' | while read line
  do
    pattern=$(echo "$line" | cut -d'"' -f2)
    comment=$(echo "$line" | sed 's|^.*", // ||')
    #echo "pattern = '$pattern', comment = '$comment'" # debug
    [ "${pattern:0:1}" = "*" ] && pattern="'$pattern'"
    echo "  - $pattern # $comment"
  done
}

#outfile=config.yaml
outfile=/dev/stdout

cat >>$outfile <<EOF
# spotify-adblock-linux/config.yaml

debug: true # print logs to stdout

blacklist:
$(header2yaml blacklist.h)

whitelist:
$(header2yaml whitelist.h)

EOF
