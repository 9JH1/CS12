set -xe
git add .  
git commit -m "$(date)"

echo ""
gcc -g main.c lib/*.c -o database  

# run options 
./database $@  

echo ""
git push
