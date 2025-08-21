git add . && git commit -m "$(date)" &>/dev/null
echo ""
gcc -g main.c lib/*.c -o database && time ./database $@  
(git push &>/dev/null &)
