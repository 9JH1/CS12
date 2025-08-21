git add . 
git commit -m "$(date)"
gcc -g main.c lib/*.c -o database && ./database $@
git push
