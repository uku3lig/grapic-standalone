#!/bin/bash

if [[ -z $1 ]];
then 
    echo "You need to pass a directory name."
    exit 1
fi

git clone --depth 1 https://github.com/uku3lig/grapic-standalone "$1"
cd "$1" || exit

rm -rf .git .gitignore LICENSE README.md setup.sh
mv main.cpp "$1.cpp"
sed -i -e "s/main/$1/g" Makefile
bash download_grapic.sh

echo "Project setup in directory '$1'"