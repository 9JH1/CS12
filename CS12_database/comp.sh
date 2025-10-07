#!/bin/bash

rm -rf src/

if [ ! -d "src" ];then 
	mkdir src 
fi 
read -p "Enter database flags: " db_flags
extra_flags="# -v -g"

if [[ "$1" = "FIXWIN" ]]; then
	icacls "." /reset /T
	takeown /f "." /r 
elif [[ "$1" = "_WIN32" ]] || [[ "$1" = "_MINGW32" ]];then
	x86_64-w64-mingw32-gcc main.c lib/*.c -o src/database $extra_flags 
	if [ $? -ne 0 ]; then 
		echo "$1 compilation failed"
		exit 1
	fi 

	if [[ "$1" = "_MINGW32" ]];then 
		wine ./src/database.exe $db_flags 
	else 
		./src/database.exe $db_flags 
	fi

else 
	if [[ !"$1" = "_NOGIT" ]];then 
		git add . &>/dev/null 
		git commit -m "$(date)" &>/dev/null 
	fi 

	gcc -g main.c lib/*.c -o src/database $extra_flags
	if [ $? -ne 0 ]; then
		echo "$1 compilation failed"
		exit 1
	fi 
	./src/database $db_flags

	if [[ ! "$1" = "_NOGIT" ]];then 
		git push &>/dev/null
	fi
fi

echo "$1 finished"
