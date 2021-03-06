/* 

HLIN608 - PROJET ALGO DU TEXTE - ALGORITHME D'ASSEMBLAGE :
- Antoine AFFLATET
- Hayaat HEBIRET
- Jérémie ROUX      
(L3 GROUPE C)  

Algorithme 1 :
Amélioration de Algorithme 0 en tenant compte des mots inclus dans les autres
Pour que m1 puisse être inclus dans m2 sans être un overlap : size(m1)<=size(m2)-2
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

vector< vector<int> > rangeMatrice(vector< vector<int> > M, int rangY, int rangX,int taille){
	int buffer = 0;
	if (rangX > rangY){
		
		for(int i=0;i<taille;i++){
			for(int j=rangY;j<taille-1;j++){
				if(j<(rangX-1)){
			 		buffer = M[i][j+1];
					M[i][j+1] = M[i][j];
					M[i][j] = buffer;
		 		}else if(j+2 < taille){
		 			buffer = M[i][j+2];
					M[i][j+2] = M[i][j];
					M[i][j] = buffer;
		 		}
			
			}
		}
	
		for(int i=rangY;i<taille-1;i++){
			for(int j=0;j<taille;j++){
			 	if(i<(rangX-1)){
			 		buffer = M[i+1][j];
					M[i+1][j] = M[i][j];
					M[i][j] = buffer;
				}else if((i+2 < taille)){
			 		buffer = M[i+2][j];
					M[i+2][j] = M[i][j];
					M[i][j] = buffer;
				}else{
			 		buffer = M[i+1][j];
					M[i+1][j] = M[i][j];
					M[i][j] = buffer;
			 	}
				if(i==j){
					M[i][j]=-1;	
				}
			}
		}
	}else{
		for(int i=0;i<taille;i++){
			for(int j=rangX;j<taille-1;j++){
				if(j<(rangY-1)){
			 		buffer = M[i][j+1];
					M[i][j+1] = M[i][j];
					M[i][j] = buffer;
		 		}else if(j+2 < taille){
		 			buffer = M[i][j+2];
					M[i][j+2] = M[i][j];
					M[i][j] = buffer;
		 		}else if(((((taille-1)-rangX)%2)!=0)||((taille-1)<=3)){
		 			buffer = M[i][j+1];
					M[i][j+1] = M[i][j];
					M[i][j] = buffer;
		 		}
			
			}
		}
	
		for(int i=rangX;i<taille-1;i++){
			for(int j=0;j<taille;j++){
			 	if(i<(rangY-1)){
			 		buffer = M[i+1][j];
					M[i+1][j] = M[i][j];
					M[i][j] = buffer;
				}else if((i+2 < taille)){
			 		buffer = M[i+2][j];
					M[i+2][j] = M[i][j];
					M[i][j] = buffer;
				}
				if(i==j){
					M[i][j]=-1;	
				}
			}
		}
	}	
	
	
	
return M;
}


int main(int argc, char *argv[]){
	if(argc==1){
		cout<<"Il faut au moins un argument"<<endl;
	}
	else{


		int nbMots = argc-1;
		string motFusion = "";

		// Récupérer et afficher la liste des mots donnés en entrée
		cout<<endl;
		vector<string> T = recuperer_mots(nbMots, argv);
		afficher_mots(nbMots, T, "RÉCUPÉRÉS");
		cout<<endl;

		// Trier le tableau selon la longueur des mots et afficher ces mots ainsi ordonnés
		cout<<endl;
		T = triMotsLongueur(nbMots, T);
		afficher_mots(nbMots, T, "TRIÉS PAR LONGUEUR");
		cout<<endl;

		// Vérifier si certains mots sont inclus dans d'autres et afficher les mots sans ces mots inclus
		cout<<endl;
		T = suppressionMotsInclus(nbMots,T);
		nbMots=T.size();
		afficher_mots(nbMots, T, "SANS LES MOTS INCLUS");
		cout<<endl;

		if(nbMots==1){
			motFusion = T[0];
		}

		vector< vector<int> > M(nbMots, vector<int>(nbMots));
		M = calculerOverlapM(T,M,nbMots);
		int ecartAffichage;
		int i,j;
		int overlapMaximum;
		
		while(nbMots>1){

			ecartAffichage = longueurMax(nbMots,T)+3;

			// Parcourir tous les mots, calculer et afficher les overlaps
			
			i = 0;
			j = 0;
			overlapMaximum = overlapMax(M,nbMots,&i,&j);
			
			cout<<"La valeur de i est égale à "<<i<<endl;
			cout<<"La valeur de j est égale à "<<j<<endl;
			
			afficherMatrice(T,M,nbMots,ecartAffichage,overlapMaximum,i,j);

			// Fusionner les overlaps et ajouter le mot fusionné
			cout<<endl;
			motFusion = fusionOverlap(i,j,overlapMaximum,T);
			cout<<endl;
				M  = rangeMatrice(M,i,j,M.size());
			if(i>j){
				T.erase(T.begin()+ i);
				T.erase(T.begin()+ j);
			}else{
				T.erase(T.begin()+ j);
				T.erase(T.begin()+ i);
			}
			M.erase(M.end()-1);
			T.push_back(motFusion);
			/*for(int z=i;z<M.size()-1;z++){
					for(int k=0;k<M.size()-1;k++){
						if(k!=z){
							if ((k<z)&&(z>=i)){
								buffer[k] = M[z][k];
								M[z][k]= M[z+1][k];
								M[z+1][k]= buffer[k];
							}else {
								buffer[k] = M[z][k];
								M[z][k]= M[z+1][k+1];
								M[z+1][k+1]= buffer[k];
							}
						}
						
					}
				}
				for(int z=j;z<M.size()-2;z++){
					for(int k=0;k<M.size()-1;k++){
						if(k!=z){
							if ((k<z)&&(z>=i)){
								buffer[k] = M[z][k];
								M[z][k]= M[z+1][k];
								M[z+1][k]= buffer[k];
							}else {
								buffer[k] = M[z][k];
								M[z][k]= M[z+1][k+1];
								M[z+1][k+1]= buffer[k];
							}
						}
						
					}
				}*/
			nbMots--;
			
		}

		cout<<"Super-chaîne : "<<motFusion<<" (Taille "<<motFusion.size()<<")"<<endl<<endl;
	}
	return 0;
}