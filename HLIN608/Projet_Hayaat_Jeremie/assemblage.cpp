#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

// Longueur maximale parmis les mots
int longueurMax(int nbMots, vector<string> T){
	int l=0;
	for(int i=0; i<nbMots; i++){
		if(l<T[i].size()){
			l=T[i].size();
		}
	}
	return l;
}

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

// Calculer les overlaps d'une matrice
vector< vector<int> > calculerOverlapM(vector<string> T, vector< vector<int> > M, int nbMots){
	for(int i=0; i<nbMots; i++){
		for(int j=0; j<nbMots; j++){
			if(i==j){
				M[i][j] = -1;
			}
			else{
				M[i][j] = calculerOverlap(T[i],T[j]);
			}
		}
	}
	return M;
}

// Afficher Matrice
void afficherMatrice(vector<string> T, vector< vector<int> > M, int nbMots, int ecart){
	for(int i=-1; i<nbMots; i++){
		for(int j=-1; j<nbMots; j++){
			if(i==-1 && j==-1){
				cout<<setw(ecart)<<" ";
			}
			else if(i==-1){
				cout<<setw(ecart)<<T[j];
			}
			else if(j==-1){
				cout<<setw(ecart)<<T[i];
			}
			else{
				cout<<setw(ecart)<<M[i][j];
			}
		}
		cout<<endl;
	}
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
		int ecartAffichage = longueurMax(nbMots,T)+3;
		cout<<endl;

		// Parcourir tous les mots, calculer et afficher les overlaps
		vector< vector<int> > M(nbMots, vector<int>(nbMots));
		M = calculerOverlapM(T,M,nbMots);
		afficherMatrice(T,M,nbMots,ecartAffichage);
	}
	return 0;
}