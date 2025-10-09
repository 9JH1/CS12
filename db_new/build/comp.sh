#!/bin/bash
build_path="build/src/"
file_name="db"
extra_flags="-DPL_VERBOSE_ENABLE" 
main_flags="main.c -o $build_path$file_name src/*.c src/remote/plib/*.c src/remote/plib/lib/*.c $extra_flags"

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
		wine "./$build_path$file_name.exe" $@
	else 
		"./$build_path$file_name.exe" $@
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
	

	"./$build_path$file_name" $@

	if [[ ! "$CALL" = "_NOGIT" ]];then 
		git push &>/dev/null &
	fi
fi

doxygen ./build/.Doxy &>/dev/null &
ln -s ./build/.Doxygen/html/index.html ./build/src/DOCUMENTATION.html &>/dev/null &
