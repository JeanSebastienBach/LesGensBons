/* 

HLIN608 - PROJET ALGO DU TEXTE - ALGORITHME D'ASSEMBLAGE :
- Antoine AFFLATET
- Hayaat HEBIRET
- Jérémie ROUX      
(L3 GROUPE C)  

Algorithme 2 :

*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;


// Afficher la liste des mots donnés en entrée
vector<string> afficher_mots(int nbMots, char *argv[]){
	cout<<"LISTE DES "<<nbMots<<" MOTS À ASSEMBLER :"<<endl;
	vector<string> T;
	for(int i=1; i<=nbMots; i++){
		T.push_back(argv[i]);
		cout<<"   "<<i<<") "<<T[i-1]<<endl;
	}
	return T;
}

// Calculer l'overlap entre deux mots 
int calculerOverlap(string x, string y){
	int lx = x.size();
	int ly = y.size();
	
	if(lx>ly){
		x=x.substr(lx-ly,ly);
		lx=x.size();
	}
	if(ly>lx){
		y=y.substr(0,ly-(ly-lx));
		ly = y.size();
	}

	bool trouve=false;
	int decalage=0;
	int result=y.size();

	while(!trouve && decalage<min(lx,ly)){
		
		trouve = (x.substr(decalage,lx)==y.substr(0,ly-decalage));
		result--;
		decalage++;
	}

	if(trouve){
		result++;
	}

	return result;
}

// Fusionner des overlaps
string fusionOverlap(int i, int j, int overlapMaximum, vector<string> T){
	string x = T[i];
	string y = T[j];
	string motFusion = "";
	string overlap = "";
	if(overlapMaximum==0){
		motFusion=x+y;
	}
	else{
		motFusion = x.substr(0,x.size()-overlapMaximum)+y;
		overlap = y.substr(0,overlapMaximum);
	}
	cout<<"fusion("<<x<<","<<y<<") = "<<motFusion<<endl;
	cout<<"-> OVERLAP : "<<overlap<<" (Taille : "<<overlap.size()<<")"<<endl;
	return motFusion;
}

int main(int argc, char *argv[]){
	if(argc==1){
		cout<<"Il faut au moins un argument"<<endl;
	}
	else{

		int nbMots = argc-1;

		// Afficher la liste des mots donnés en entrée
		cout<<endl;
		vector<string> T = afficher_mots(nbMots, argv);
		
		string resultat ="";
		int max = -1;
		int maxIndiceI = -1;
		int maxIndiceJ = -1;
		int buf = -1;
		while(T.size()>1){
			for(int j=0; j<T.size()-1; j++){
				buf = calculerOverlap(T[T.size()-1],T[j]);
				if(buf>max){
					max = buf;
					maxIndiceI = T.size()-1;
					maxIndiceJ = j;
				}
				buf = calculerOverlap(T[j],T[T.size()-1]);
				if(buf>max){
					max = buf;
					maxIndiceI = j;
					maxIndiceJ = T.size()-1;
				}
			}
			
			T.push_back(fusionOverlap(maxIndiceI, maxIndiceJ, max, T));
			T.erase(T.begin()+ maxIndiceI);
			T.erase(T.begin()+ maxIndiceJ);
			max = -1;
			maxIndiceI = -1;
			maxIndiceJ = -1;
			buf = -1;
		}	
		resultat = T[0];
		

		cout<<"Super-chaîne : "<<resultat<<" (Taille "<<resultat.size()<<")"<<endl<<endl;
	}
	return 0;
}