#!/bin/sh

# qtc-buildpackage: create a proper debian source tarball from a qtc project dir

# To use, invoke qtc-buildpackage in Scratchbox with the current directory being
# a Harmattan or Fremantle Qt Creator project (thus containing qtc_packaging
# folder).

set -e

# Get the current target name from Scratchbox (FREMANTLE, HARMATTAN)
target_arch=`sb-conf current`
target=${target_arch/_*/}
target=`tr "[:upper:]" "[:lower:]" <<< "$target"`

# Use it to decide the required qtc_packaging sub directory.
qtc_packaging_dir="qtc_packaging/debian_$target"
changelog="$qtc_packaging_dir/changelog"

# Some sanity checks
if [ ! -d "$qtc_packaging_dir" ]; then
	echo "Error: $target packaging not found"
	exit 1
fi

if [ ! -r "$changelog" ]; then
	echo "Error: no changelog in $qtc_packaging_dir"
	exit 1
fi

# Parse the changelog to obtain package name and version
info=`dpkg-parsechangelog -l"$changelog"`
package=`echo "$info" | grep ^Source\: | cut -f2- -d" "`
version=`echo "$info" | grep ^Version\: | cut -f2- -d" "`

if [ -z "$version" ]; then
	echo "Error: unable to parse version from $changelog"
	exit 1
elif [ -z "$package" ]; then
	echo "Error: unable to parse package name from $changelog"
	exit 1
fi

# Create a temporary dir for building
tmp_dir=`mktemp -d`

# Make a temporary copy of the package directory with the proper name
package_tmp_dir="$tmp_dir/$package-$version"
cp -a . "$package_tmp_dir"

# Copy the appropiate package for the current scratchbox target
cp -a "$qtc_packaging_dir" "$package_tmp_dir/debian"

# Patch the rules file to readd what QtC removes
sed -e 's/^\([\t ]*\)#[\t ]*\(.*\)[\t ]*# Uncomment this line for use without Qt Creator/\1\2/g' -i "$package_tmp_dir/debian/rules"

# Add the aegis manifest file if not empty
if [ -r "$package_tmp_dir/debian/manifest.aegis" ]; then
	if [ -s "$package_tmp_dir/debian/manifest.aegis" ]; then
		mv "$package_tmp_dir/debian/manifest.aegis" "$package_tmp_dir/debian/$package.aegis"
	else
		rm "$package_tmp_dir/debian/manifest.aegis"
	fi
fi

# Now build the source package
(cd "$package_tmp_dir" && dpkg-buildpackage -rfakeroot -S -sa -i -I.git -I.svn)

# Remove the build directory
rm -rf "$package_tmp_dir"

# Move back the results to the parent directory
mv $tmp_dir/* ..

# And now delete the temporary directory
rmdir $tmp_dir

exit 0

