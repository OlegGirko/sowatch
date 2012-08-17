#!/bin/bash

set -e

allfiles=$(echo *.png | sort)
atlasfiles=""
stems=""

for i in $allfiles
do
	echo $i
	if [[ $i =~ ^([a-z]+)[1-9].png ]]
	then
		stem=${BASH_REMATCH[1]}
		output=$stem.png.tmp
		if [[ $atlasfiles != *$output* ]]
		then
			montage $stem?.png -geometry 32x32 $output
			atlasfiles="$atlasfiles $output"
			stems="$stems $stem"
		fi
	fi
done

montage $atlasfiles -geometry 64x32+0+0 -gravity NorthWest -tile 1x atlas.png

rm -f $atlasfiles

echo $stems | tr ' ' '\n' > atlas.txt

exit 0

