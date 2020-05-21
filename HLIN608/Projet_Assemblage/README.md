POUR TESTER L'EXECUTION DE TOUS LES PROGRAMMES

- Générer 10 mots sur l'alphabet A,C,G,T dont vous définisez la longueur
./generation.sh "longueur des 10 mots à générer"

- Pour faire des assemblages selon version de l'algo (0,1 ou 2) et liste des mots à assembler
./assemblage.sh versionAlgo mot1 mot2 mot3 ... motN

- Générer les fichiers d'évolution de la taille de la super-séquence et gain associé -> GainSequenceEvo.txt et GainPourcentageEvo.txt
./tailleGainSequenceEvo.sh

- Générer les fichiers pour des tailles de la super-séquence et gain associé -> dossiers tailleSupersequence et gainSupersequence
./tailleGainSequence.sh "longueur des 10 mots à générer" "nombre d'assemblages"

- Générer un fichier avec l'évolution des temps de calcul pour les 3 implémentations de l'algo -> dossier tempsCalcul
./temps.sh