#!/bin/bash

set -e

MYDIR=$(dirname $0)
cd "$MYDIR/../sidplay-libs-2.1.1"

# compile
CXXFLAGS="-O2 -ffast-math" \
DEB_BUILD_OPTIONS="debug nostrip" \
MAKEFLAGS=-j3 \
fakeroot debian/rules binary-arch

# install
cd ..
sudo dpkg -i *.deb
