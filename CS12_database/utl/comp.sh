#!/bin/bash

rm -rf src/

if [ ! -d "src" ];then 
	mkdir src 
fi 

CALL="$1"
extra_flags="-Wall -Wextra"
main_flags="main.c lib/*.c lib/hlib/lib/draw.c -o src/database $extra_flags"

if [[ "$CALL" = "_WIN32" ]] || [[ "$CALL" = "_MINGW32" ]];then
	shift 
	
	if [[ "$CALL" = "_MINGW32" ]];then 
		x86_64-w64-mingw32-gcc $main_flags 
	else 
		gcc main.c $main_flags
	fi


	if [ $? -ne 0 ]; then 
		echo "$1 compilation failed"
		exit 1
	fi 

	shift 
	
	if [[ "$CALL" = "_MINGW32" ]];then 
		wine ./src/database.exe $@ || echo "database failed to run"
	else 
		"./src/database.exe" $@ || echo "database failed to run"
	fi

else 
	if [[ ! "$CALL" = "NOGIT" ]];then 
		git add . &>/dev/null 
		git commit -m "$(date)" &>/dev/null 
		shift 
	fi


	gcc $main_flags
	
	if [ $? -ne 0 ]; then
		echo "$1 compilation failed"
		exit 1	
	fi 
	
	shift 

	./src/database "$@" || echo "database failed to run"

	if [[ ! "$CALL" = "_NOGIT" ]];then 
		git push &>/dev/null
	fi
fi

echo "$CALL finished"
