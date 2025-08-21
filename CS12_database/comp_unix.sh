git add . && git commit -m "$(date)" &>/dev/null
echo ""
gcc -g main.c lib/*.c -o database && ./database $@  
git push
