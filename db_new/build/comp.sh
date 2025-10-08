#!/bin/bash

build_path="build/src/"
extra_flags="-Wall -Wextra -Wno-unused-function -Wno-unused-variable"
file_name="db"
main_flags="main.c src/*.c src/remote/plib/*.c -o $build_path$file_name $extra_flags"

# handle path
rm -rf "$build_path"
if [ ! -d "$build_path" ];then 
	mkdir "$build_path"
fi 

CALL="$1"

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
		wine "./$build_path$file_name.exe" $@ || echo "database exited badly"
	else 
		"./$build_path$file_name.exe" $@ || echo "database exited badly"
	fi

else 
	if [[ ! "$CALL" = "NOGIT" ]];then 
		git add . &>/dev/null 
		git commit -m "$(date)" &>/dev/null 
	else 
		shift 
	fi


	gcc $main_flags
	
	if [ $? -ne 0 ]; then
		echo "$1 compilation failed"
		exit 1	
	fi 
	
	shift 

	"./$build_path$file_name" "$@" || echo "database exited badly"

	if [[ ! "$CALL" = "_NOGIT" ]];then 
		git push &>/dev/null
	fi
fi

echo "$1 finished"
