set -xe
git add .  
git commit -m "$(date)"

echo ""
gcc -g main.c lib/*.c -o database  

#show options 
./database 

# run options 
./database $@  
echo ""
git push
