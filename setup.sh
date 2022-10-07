#!/bin/bash

if [[ -z $1 ]];
then 
    echo "You need to pass a directory name."
    exit 1
fi

git clone --depth 1 https://github.com/uku3lig/grapic-standalone "$1"
cd "$1" || exit

bash download_grapic.sh
rm -rf .git .gitignore LICENSE README.md setup.sh download_grapic.sh

mv main.cpp "$1.cpp"
sed -i -e "s/main/$1/g" Makefile

echo "Project setup in directory '$1'"