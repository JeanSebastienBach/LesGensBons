# Utilisation : ./tailleGainSequence.sh "longueur des 10 mots à générer" "nombre d'assemblages"

clear
g++ tailleGainSequence.cpp -Wall -o tailleGainSequence
./tailleGainSequence $1 $2