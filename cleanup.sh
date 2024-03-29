#!/bin/sh

make -C src clean
make -C data clean

rm -rf autom4te.cache build .deps src/.deps
rm -f aclocal.m4 compile config.guess config.sub configure depcomp install-sh Makefile.in missing configure~ Makefile src/Makefile.in data/Makefile.in src/Makefile data/Makefile config.log config.status config.guess~ config.sub~ install-sh~
