// HLIN608 - PARTIEL A DISTANCE - ALGO DU TEXTE
// Jérémie ROUX - L3 Groupe C

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

vector< vector<int> > dotplot(string a, string b){
	vector< vector<int> > M(a.size(), vector<int>(b.size()));
	for(int i=0; i<a.size(); i++){
		for(int j=0; j<b.size(); j++){
			if(a[i]==b[j]){
				M[i][j]=1;
			}
			else{
				M[i][j]=0;
			}
		}
	}
	return M;
}

void exportMatrice(vector< vector<int> > M, string mot1, string mot2, string nom){
	string const nomFichier(nom);
    ofstream monFlux(nomFichier.c_str());

    if(monFlux){
        monFlux << "P1" << endl;
        monFlux << mot2.size() << " " << mot1.size() << endl;
        for(int i=0; i<mot1.size(); i++){
			for(int j=0; j<mot2.size(); j++){
				monFlux << M[i][j] << " ";
			}
			monFlux << endl;
		}
    }
    else{
        cout << "ERREUR : Impossible d'ouvrir le fichier." << endl;
    }
}

vector< vector<int> > dotplotFiltre(string s, string t, int l){
	int n = s.size();
	int m = t.size();
	vector< vector<int> > M(n, vector<int>(m));
	
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			M[i][j]=0;
		}
	}

	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			int k=0;
			while(i+k<n && j+k<m && s[i+k]==t[j+k]){
				k++;
			}
			if(k>=l){
				k=0;
				while(i+k<n && j+k<m && s[i+k]==t[j+k]){
					M[i+k][j+k]=1;
					k++;
				}
			}
		}
	}
	return M;
}

vector< vector<int> > dotplot2Filtre(string s, string t, int l){
	int n = s.size();
	int m = t.size();
	vector< vector<int> > M(n, vector<int>(m));
	
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			M[i][j]=0;
		}
	}

	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			int k=0;
			while(i+k<n && j+k<m && s[i+k]==t[j+k]){
				k++;
			}
			if(k>=l){
				k=0;
				while(i+k<n && j+k<m && s[i+k]==t[j+k]){
					M[i+k][j+k]=1;
					k++;
				}
			}
		}
	}

	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			int k=0;
			while(i+k<n && j-k<m && s[i+k]==t[j-k]){
				k++;
			}
			if(k>=l){
				k=0;
				while(i+k<n && j-k<m && s[i+k]==t[j-k]){
					M[i+k][j-k]=1;
					k++;
				}
			}
		}
	}

	return M;
}



int main(int argc, char *argv[]){

	string mot1 = "NATATION";
	string mot2 = "EQUITATION";
	vector< vector<int> > M = dotplot(mot1,mot2);
	exportMatrice(M,mot1,mot2,"matriceBinaire.pbm");

	string seq1 = "ROUXROUXJEREMIEJEREMIE";
	string seq2 = "ROUXEIMEREJROUXJEREMIE";
	vector< vector<int> > M2 = dotplot(seq1,seq2);
	exportMatrice(M2,seq1,seq2,"dotplot.pbm");

	vector< vector<int> > M3 = dotplotFiltre(seq1,seq2,3);
	exportMatrice(M3,seq1,seq2,"dotplotfiltre.pbm");

	vector< vector<int> > M4 = dotplot2Filtre(seq1,seq2,3);
	exportMatrice(M4,seq1,seq2,"dotplot2filtre.pbm");

    return 0;
}