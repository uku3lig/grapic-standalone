#!/bin/bash

BASE_URL='https://raw.githubusercontent.com/ucacaxm/grapic/master'
FILES=('src/Grapic.cpp' 'src/Grapic.h' 'data/ttf/Roboto-Regular.ttf')

for fname in "${FILES[@]}"
do
    echo "Downloading '$fname'..."
    mkdir -p "${fname%/*}"
    curl -fsSL "$BASE_URL/$fname" -o "$fname"
done
echo "Successfully downloaded Grapic."