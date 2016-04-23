#!/bin/sh
#echo "setting up enviorment"
#aclocal >/dev/null 2>&1
#autoheader
#glibtoolize 
#automake  --foreign --add-missing --copy 
#autoconf >/dev/null 
autoreconf -i
echo "complete "
