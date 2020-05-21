/* 

HLIN608 - PROJET ALGO DU TEXTE - ALGORITHME D'ASSEMBLAGE :
- Antoine AFFLATET
- Hayaat HEBIRET
- Jérémie ROUX      
(L3 GROUPE C)  

Algorithme Naif permettant de calculer l'éxacte plus courte séquence contenant le moins de caractères:

*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

// Afficher un tableau
void afficherTab(vector<string> T){
	for(int i=0; i<=T.size(); i++){
		if(i==0){
			cout<<endl<<"[ ";
		}
		if(i==T.size()){
			cout<<"]"<<endl;
		}
		else{
			cout<<T[i]<<" ";
		}
	}
}

// Ajouter un élement en tête d'un tableau
vector<string> ajouterTete(string x, vector<string> T){
	vector<string> resultT;
	resultT.push_back(x);
	for(int i=0; i<T.size(); i++){
		resultT.push_back(T[i]);
	}
	return resultT;
}

// Tableau sans sa tête
vector<string> sansTete(vector<string> T){
	vector<string> resultT;
	for(int i=1; i<T.size(); i++){
		resultT.push_back(T[i]);
	}
	return resultT;
}

// Fusionner 2 tableaux triés
vector<string> fusion(vector<string> T1, vector<string> T2){
	if(T1.size()==0){
		return T2;
	}
	else if(T2.size()==0){
		return T1;
	}
	else if(T1[0].size()<=T2[0].size()){
		return ajouterTete(T1[0], fusion(sansTete(T1),T2));
	}
	else{
		return ajouterTete(T2[0], fusion(T1,sansTete(T2)));
	}
}

// Classer les mots selon leur longueur (tri fusion)
vector<string> triMotsLongueur(int nbMots, vector<string> T){
	if(nbMots<=1){
		return T;
	}

	vector<string> T1,T2;
	for(int i=0; i<nbMots; i++){
		if(i<=(nbMots/2)-1){
			T1.push_back(T[i]);
		}
		else{
			T2.push_back(T[i]);
		}
	}
	return fusion(triMotsLongueur(T1.size(),T1),triMotsLongueur(T2.size(),T2));
}

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

// Récupérer la liste des mots donnés en ligne de commande
vector<string> recuperer_mots(int nbMots, char *argv[]){
	vector<string> T;
	for(int i=1; i<=nbMots; i++){
		T.push_back(argv[i]);
	}
	return T;
}

// Afficher la liste des mots donnés en entrée
void afficher_mots(int nbMots, vector<string> T, string description){
	cout<<"LISTE DES "<<nbMots<<" MOTS "<<description<<" :"<<endl;
	for(int i=0; i<nbMots; i++){
		cout<<"   "<<i+1<<") "<<T[i]<<endl;
	}
}

// Le mot x est sous-mot de y
bool estSousMot(string x, string y){
	
	// S'il font la même taille
	if(x.size()==y.size()){
		return x==y;
	}

	/*if(x.size()==1){
		for(int i=0; i<y.size(); i++){
			if(x[0]==y[i])
				return true;
		}
		return false;
	}*/

	bool result=false;
	int diff=y.size()-x.size();
	int i=0;
	while(!result && i<=diff){
		result = (x==y.substr(i,x.size()));
		i++;
	}

	return result;
}

// Suppression des mots inclus dans d'autres
vector<string> suppressionMotsInclus(int nbMots,vector<string> T){
	vector<int> S; // Liste des indices des sous-mots
	for(int i=0; i<nbMots; i++){
		bool trouve=false;
		for(int j=i+1; j<nbMots; j++){
			if(estSousMot(T[i],T[j]) && !trouve){
				S.push_back(i);
				cout<<T[i]<<" ("<<i+1<<") est inclus dans "<<T[j]<<" ("<<j+1<<")"<<endl;
				trouve=true;
			}
		}
	}
	cout<<endl;

	// Suppression des doublons remarqués
	for(int i=S.size()-1; i>=0; i--){
		T.erase(T.begin()+ S[i]);
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
string fusionOverlap(string x, string y, int overlapMaximum){
	string motFusion = "";
	string overlap = "";
	if(overlapMaximum==0){
		motFusion=x+y;
	}
	else{
		motFusion = x.substr(0,x.size()-overlapMaximum)+y;
		overlap = y.substr(0,overlapMaximum);
	}
	//cout<<"fusion("<<x<<","<<y<<") = "<<motFusion<<endl;
	//cout<<"-> OVERLAP : "<<overlap<<" (Taille : "<<overlap.size()<<")"<<endl;
	return motFusion;
}




string reconstructionSuperChaine(vector<int> &tab,vector<string> T){
	for(int g=0;g<tab.size();g++){
		cout<<"|"<<tab[g]<<"|";
	}
	cout<<endl;
	string resultat="";
	int overlap;
	for(int i=0;i<T.size();i++){
		for(int j=0;j<T.size();j++){
			if(tab[j]==i){
				cout<<" resultat = "<<resultat<<", T["<<j<<"] = "<<T[j]<<endl;
				overlap=calculerOverlap(resultat, T[j]);
				resultat=fusionOverlap(resultat,T[j],overlap);
			}
		}
	}

	return resultat;
}

void calculMin(vector<string> T,vector<int> &tab,int profondeur,int &minLocal,string &resMax,string &res){

	bool possible;
	for(int p=profondeur;p<T.size();p++){	
		tab[p]=-1;
	}
	
	for(int i=0;i<T.size();i++){
		
		possible = true;
		for(int j=0;j<profondeur;j++){
			if (tab[j]==i){
				possible = false;
			}
		}
		if(possible){
			tab[profondeur]=i;
			if (profondeur < (T.size()-1)){
				profondeur++;
				calculMin(T,tab,profondeur,minLocal,resMax,res);
			}else{
				res = reconstructionSuperChaine(tab,T);
			}
			if(res.size() < minLocal){
				cout<<"----------------------------"<<endl;
				cout<<"Ancien minimum = "<<minLocal<<" nouveau résultat = "<<res.size()<<endl;
				cout<<"----------------------------"<<endl;
				minLocal = res.size();
				resMax = res;
			}
		}
	}

	for(int g=0;g<tab.size();g++){
		tab[g]=-1;
	}
}



int main(int argc, char *argv[]){
	if(argc<=1){
		cout<<"Il faut au moins deux arguments"<<endl;
	}
	else{

		int nbMots = argc-1;
		string motFusion = "";

		// Récupérer et afficher la liste des mots donnés en entrée
		cout<<endl;
		vector<string> T = recuperer_mots(nbMots, argv);
		afficher_mots(nbMots, T, "RÉCUPÉRÉS");
		cout<<endl;

		
		if(nbMots==1){
			motFusion = T[0];
		}
		int minLocal = 99999;
		string res ="";
		vector<int> choixMin;
		
		//remplir les nbMots avec des -1
		for(int i=0;i<T.size();i++){
			choixMin.push_back(-1);
		}
		
		calculMin(T,choixMin,0,minLocal,motFusion,res);
		
		cout<<"Super-chaîne : "<<motFusion<<" (Taille "<<motFusion.size()<<")"<<endl<<endl;
	}
	return 0;
}