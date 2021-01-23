# !/bin/sh
echo package list
sudo dpkg --get-selections | cut -f1 | grep 'libc\|g++\|build-essential'
echo start to reinstall all C and CPP packages
for package in $(sudo dpkg --get-selections | cut -f1 | grep 'gcc\|libc\|g++\|build-essential')
do
    sudo apt-get install --reinstall -y $package
done