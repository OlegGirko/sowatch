#!/bin/bash

function make_symlink {
	local src="$1"
	local target="$2"
	if [ -L "$target/$(basename "$src")" ]; then
		# Overwrite existing symlinks
		ln -sf "$src" "$target"
	else
		# But nothing else
		ln -s "$src" "$target"
	fi
}

function make_symlinks {
	local target="${@:$#}"
	while [ $# -gt 1 ]; do
		local src="$1"
		shift
		if [[ "$src" == *\** ]]; then
			continue
		fi
		make_symlink "$src" "$target"
	done
}

SCRIPT_PATH=$(readlink -f "$0")
SOWATCH_ROOT=$(dirname "$SCRIPT_PATH")
BUILD_ROOT=$(pwd)

rm -r drivers notifications watchlets
mkdir -p drivers notifications watchlets

make_symlinks $BUILD_ROOT/*/lib*driver.so drivers
make_symlinks $BUILD_ROOT/*/lib*notification.so notifications
make_symlinks $BUILD_ROOT/*/lib*watchlet.so watchlets

for i in $SOWATCH_ROOT/*; do
	b="$BUILD_ROOT/$(basename $i)"
	if [ -d "$i" -a -d "$b" ]; then
		make_symlinks $i/*.png $i/*.qml "$b"
		if [ -d "$i/res" ]; then
			make_symlinks $i/res/* "$b"
		fi
	fi
	
done
