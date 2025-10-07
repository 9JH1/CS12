#!/bin/bash
if [ ! -d "src" ];then 
	mkdir src 
fi 
read -p "Enter database flags: " db_flags

if [[ "$1" = "_WIN32" ]] || [[ "$1" = "_MINGW32" ]];then
	x86_64-w64-mingw32-gcc -g main.c lib/*.c -g -o src/database -O2 # -v
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

	gcc -g main.c lib/*.c -g -o src/database -O2 # -v
	./src/database $db_flags

	if [[ ! "$1" = "_NOGIT" ]];then 
		git push &>/dev/null
	fi
	echo "Done!"
fi
