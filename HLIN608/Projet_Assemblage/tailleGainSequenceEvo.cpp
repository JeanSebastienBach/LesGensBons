/* 
HLIN608 - PROJET ALGO DU TEXTE - ALGORITHME D'ASSEMBLAGE :
- Antoine AFFLATET
- Hayaat HEBIRET
- Jérémie ROUX      
(L3 GROUPE C)  
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <fstream>

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
	return motFusion;
}

int main(int argc, char *argv[]){	
	string G0 = "";
	string GP0 = "";

	int nb = 100;
	srand(time(NULL));

	for(int t=2; t<=100; t++){

		string chaineDebut = "\\addplot\n table[row sep=\\\\,y index=0] {\n  ";
		G0.append(chaineDebut);
		GP0.append(chaineDebut);

		int nbMots = 10;
		for(int n=0; n<nb; n++){

			// Génération des séquences à assembler
			vector<string> T;
			for(int i=1; i<=nbMots; i++){
				T.push_back(generation(t));
			}

			// Optimisation 0
			vector<string> T0 = T;
			int nbMots0 = nbMots;

			string motFusion0 = "";
			if(nbMots0==1){
				motFusion0 = T[0];
			}
			while(nbMots0>1){
				// Parcourir tous les mots et calculer 
				vector< vector<int> > M(nbMots0, vector<int>(nbMots0));
				M = calculerOverlapM(T0,M,nbMots0);
				int i,j;
				int overlapMaximum = overlapMax(M,nbMots0,&i,&j);

				// Fusionner les overlaps et ajouter le mot fusionné
				motFusion0 = fusionOverlap(i,j,overlapMaximum,T0);

				if(i>j){
					T0.erase(T0.begin()+ i);
					T0.erase(T0.begin()+ j);
				}
				else{
					T0.erase(T0.begin()+ j);
					T0.erase(T0.begin()+ i);
				}
				T0.push_back(motFusion0);
				nbMots0--;
			}

			G0.append(to_string((t*10)-motFusion0.size()));
			G0.append("\\\\");

			GP0.append(to_string(100-((motFusion0.size()/(t*10.))*100.)));
			GP0.append("\\\\");
		}

		string chaineFin = "\n };\n\n\n";
		G0.append(chaineFin);
		GP0.append(chaineFin);

		cout<<"Taille : "<<t<<endl;
	}

    // Gain superséquence
    string const nomFichier2("GainSequenceEvo_lg.txt");
	ofstream monFlux2(nomFichier2.c_str());

    if(monFlux2){
    	monFlux2 << "Evolution du gain de nucléotides avec des tailles de 2 à 100"<<endl<<endl;
        
        monFlux2<<G0<<endl<<endl;

    }
    else{
        cout << "ERREUR : Impossible d'ouvrir le fichier." << endl;
    }

    // Gain en pourcentage superséquence
    string const nomFichier3("GainPourcentageSequenceEvo.txt");
	ofstream monFlux3(nomFichier3.c_str());

    if(monFlux3){
    	monFlux3 << "Evolution du pourcentage de gain de nucléotides avec des tailles de 2 à 100"<<endl<<endl;
        
        monFlux3<<GP0<<endl<<endl;

    }
    else{
        cout << "ERREUR : Impossible d'ouvrir le fichier." << endl;
    }

	return 0;
}