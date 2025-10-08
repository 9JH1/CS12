@echo off
wsl.exe --install archlinux
wsl.exe pacman -Sy --needed gcc git
wsl.exe git clone https://github.com/9jh1/CS12 && ./CS12/CS12_database/src/comp.sh NOGIT
