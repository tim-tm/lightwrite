#!/bin/sh

set -xe

autoreconf --force --install
mkdir -p build
cd build
../configure
