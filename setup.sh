#!/bin/bash

if [[ -z $1 ]];
then 
    echo "No directory name given, using '.'"
    GRAPIC_DIRNAME="."
else
    GRAPIC_DIRNAME=$1
fi

git clone --depth 1 https://github.com/uku3lig/grapic-standalone "$GRAPIC_DIRNAME"
cd "$GRAPIC_DIRNAME" || exit

bash download_grapic.sh
rm -rf .git .gitignore LICENSE README.md setup.sh download_grapic.sh

if [[ -n $1 ]];
then
    mv main.cpp "$1.cpp"
    sed -i -e "s/main/$1/g" Makefile
fi

echo "Project setup in directory '$GRAPIC_DIRNAME'"
