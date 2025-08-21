git add . 
git commit -m "$(date)"
echo ""
gcc -g main.c lib/*.c -o database && ./database $@ && git push &>/dev/null 
