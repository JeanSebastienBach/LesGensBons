/* 

HLIN608 - PROJET ALGO DU TEXTE - ALGORITHME D'ASSEMBLAGE :
- Antoine AFFLATET
- Hayaat HEBIRET
- Jérémie ROUX      
(L3 GROUPE C)  

Generation :
Programme qui génére 10 mots de longueurs définies par l'utilisateur
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
#include <time.h>

using namespace std;

string nucleotideHasard(float matriceProba[]){
	int p = rand() % 1001;
	if(p<=matriceProba[0]*1000){
		return "A";
	}
	else if(p<=(matriceProba[0]+matriceProba[1])*1000){
		return "C";
	}
	else if(p<=(matriceProba[0]+matriceProba[1]+matriceProba[2])*1000){
		return "G";
	}
	else{
		return "T";
	}
}

string generation(int t){
	string sequence = "";
	int x=0;

	if(t>1){
		// Nucléotide de départ 
		float teteSequence[4] = {0.250,0.250,0.250,0.250};
		sequence += nucleotideHasard(teteSequence);
		x++;
	}

	while(t>x){
		char dernierNucleotide = sequence.back();
		float sequenceA[4] = {0.300,0.205,0.285,0.210};
		float sequenceC[4] = {0.322,0.298,0.078,0.302};
		float sequenceG[4] = {0.248,0.246,0.298,0.208};
		float sequenceT[4] = {0.177,0.239,0.292,0.292};

		if(dernierNucleotide=='A'){
			sequence += nucleotideHasard(sequenceA);
		}
		else if(dernierNucleotide=='C'){
			sequence += nucleotideHasard(sequenceC);
		}
		else if(dernierNucleotide=='G'){
			sequence += nucleotideHasard(sequenceG);
		}
		else if(dernierNucleotide=='T'){
			sequence += nucleotideHasard(sequenceT);
		}
		x++;
	}

	return sequence;
}

int main(int argc, char *argv[]){
	if(argc!=2){
		cout<<"Il faut deux arguments"<<endl;
	}
	else{
		cout<<endl;
		srand(time(NULL));
		int t = stoi(argv[1]);
		for(int i=1; i<=10; i++){
			cout<<i<<") "<<generation(t)<<endl;
		}
		cout<<endl;
	}
	
	return 0;
}