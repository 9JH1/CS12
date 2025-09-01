git add . &>/dev/null 
git commit -m "$(date)" &>/dev/null 
gcc -g main.c lib/*.c -Wall -Wextra -g -o database  && ./database $@  
git push
