#!/bin/sh -f
#
# Copyright (c) 2000-2001,2005 Silicon Graphics, Inc.  All Rights Reserved.
#

OPTS=" -u"
VERBOSE=''
PATH="/bin:/usr/bin:/usr/sbin"
USAGE="Usage: xfsrq [-guv] dumpfile"

while getopts "guv" c
do
	case $c in
	g)	OPTS=" -g";;
	u)	OPTS=" -u";;
	v)	VERBOSE=true;;
	\?)	echo $USAGE 1>&2
		exit 2
		;;
	esac
done

_error()
{
	echo "Error: $@"
	exit 1
}
[ -x /usr/bin/perl ] || _error "cannot find /usr/bin/perl"
[ -x /usr/bin/expr ] || _error "cannot find /usr/bin/expr"
[ -x /usr/sbin/setquota ] || _error "cannot find /usr/sbin/setquota"

set -- extra $@
shift $OPTIND
case $# in
	1)	perl -pe 's/^fs = (.*)/\1 / && chomp' < $1 | \
		while read fs id bsoft bhard isoft ihard; do
			[ $VERBOSE ] && echo setting quota for id=$id dev=$fs
			# blk conversion (512 -> 1024)
			bsoft=`expr $bsoft / 2`
			bhard=`expr $bhard / 2`
			setquota $OPTS $id $bsoft $bhard $isoft $ihard $fs
		done
		;;
	*)	echo $USAGE 1>&2
		exit 2
		;;
esac
exit 0
