# Utilisation : ./assemblage.sh versionAlgo mot1 mot2 mot3 ... motN

clear
g++ assemblage$1.cpp -Wall -o assemblage$1
./assemblage$1 ${@:2}