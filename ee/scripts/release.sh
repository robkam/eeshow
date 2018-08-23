#!/bin/sh
BASE=pub/kicad/proto_v2

usage()
{
	echo "usage: $0 yyyy-mm-dd" 1>&2
	exit 1
}


d="[0123456789]"
[ "$1" ] || usage
[ "${1#$d$d$d$d-$d$d-$d$d}" ] && usage
[ "$2" ] && usage

TZ=UTC eeplot -n -d "%{%F %T}s" HEAD:neo900.pro -o neo900.pdf || exit
echo mkdir $BASE/$1 | neo900 || exit
neo900 neo900.pdf $BASE/$1/neo900__proto_v2__by_eeshow.pdf
echo chmod g+w -R $BASE/$1 | neo900
