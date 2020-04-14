/* 

HLIN608 - PROJET ALGO DU TEXTE - ALGORITHME D'ASSEMBLAGE :
- Antoine AFFLATET
- Hayaat HEBIRET
- Jérémie ROUX      
(L3 GROUPE C)  

Algorithme 0 :
Il ne tient pas compte qu'il peut y avoir des mots sous-mots d'autres au début
*/

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

// Calculer l'overlap max
int overlapMax(vector< vector<int> > M, int nbMots, int *vali, int *valj){
	int max=-1;
	for(int i=0; i<nbMots; i++){
		for(int j=0; j<nbMots; j++){
			if(M[i][j]>max){
				max=M[i][j];
				*vali=i;
				*valj=j;
			}
		}
	}
	return max;
}

// Afficher Matrice
void afficherMatrice(vector<string> T, vector< vector<int> > M, int nbMots, int ecart, int overlapMax, int x, int y){
	bool maxTrouve=false;
	for(int i=-1; i<nbMots; i++){
		for(int j=-1; j<nbMots; j++){
			if(i==-1 && j==-1){
				cout<<setw(ecart)<<" ";
			}
			else if(i==-1){
				if(j==y){
					string mot = T[j];
					int taille = mot.size();
					cout<<setw(ecart)<<"\033[1;34m"<<mot.substr(0,overlapMax)<<"\033[0m"<<mot.substr(overlapMax,taille);
				}
				else{
					cout<<setw(ecart)<<T[j];
				}
			}
			else if(j==-1){
				if(i==x){
					string mot = T[i];
					int taille = mot.size();
					cout<<setw(ecart-overlapMax)<<mot.substr(0,taille-overlapMax)<<"\033[1;34m"<<mot.substr(taille-overlapMax,taille)<<"\033[0m";
				}
				else{
					cout<<setw(ecart)<<T[i];
				}
			}
			else{
				if(!maxTrouve && M[i][j]==overlapMax){
					cout<<"\033[1;31m"<<setw(ecart)<<M[i][j]<<"\033[0m";
					maxTrouve=true;
				}
				else{
					cout<<setw(ecart)<<M[i][j];
				}
			}
		}
		cout<<endl;
	}
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
		string motFusion = "";
		cout<<endl;

		while(nbMots>1){

			int ecartAffichage = longueurMax(nbMots,T)+3;

			// Parcourir tous les mots, calculer et afficher les overlaps
			vector< vector<int> > M(nbMots, vector<int>(nbMots));
			M = calculerOverlapM(T,M,nbMots);
			int i,j;
			int overlapMaximum = overlapMax(M,nbMots,&i,&j);
			afficherMatrice(T,M,nbMots,ecartAffichage,overlapMaximum,i,j);

			// Fusionner les overlaps et ajouter le mot fusionné
			cout<<endl;
			motFusion = fusionOverlap(i,j,overlapMaximum,T);
			cout<<endl;

			if(i>j){
				T.erase(T.begin()+ i);
				T.erase(T.begin()+ j);
			}
			else{
				T.erase(T.begin()+ j);
				T.erase(T.begin()+ i);
			}
			T.push_back(motFusion);
			nbMots--;
		}

		cout<<"Super-chaîne : "<<motFusion<<" (Taille "<<motFusion.size()<<")"<<endl<<endl;
	}
	return 0;
}