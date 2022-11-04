#! /bin/sh

make

#move/copy build files to build folder
mv -f EBOOT.PBP 1.0/FTPc
#cp -rf src/img/* 1.0/FTPc/img

#make 1.5 release
#./make15.bat
#cp -rf src/img/* 1.5/FTPc/img

#cleanup unused files
rm -rf src/*.o
rm -rf *.*~


