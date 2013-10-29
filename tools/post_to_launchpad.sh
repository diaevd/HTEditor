#!/bin/bash

set -x
./autogen
./configure
make
make htdoc.h
make
make distdir
_VERSION=$(grep -E '^VERSION = ' Makefile|sed -re 's/^VERSION = (.+)$/\1/')
_PACKAGE=$(grep -E '^PACKAGE = ' Makefile|sed -re 's/^PACKAGE = (.+)$/\1/')
echo $_VERSION
echo $_PACKAGE
cd ${_PACKAGE}-${_VERSION}
cp -a ../debian .
debuild -k0xD638771E -S # create and sign source package
cd ..
dput ppa:diaevd/ppa hteditor_${_VERSION}~ubuntu*.changes # push it to launchpad
rm hteditor_${_VERSION}~ubuntu*.dsc
rm hteditor_${_VERSION}~ubuntu*_source.build
rm hteditor_${_VERSION}~ubuntu*_source.changes
rm hteditor_${_VERSION}~ubuntu*.tar.gz
rm hteditor_${_VERSION}~ubuntu*_source.ppa.upload
rm -rf ${_PACKAGE}-${_VERSION}
