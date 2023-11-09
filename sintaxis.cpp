#ifndef sintaxis_Cabecera
#define sintaxis_Cabecera
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>
using namespace std;
bool registro(map<string,int> &traductor,vector<set<int> > &primeros,nodo_TRIE *arbol,int cantidadTokens){
    ifstream fin;
    fin.open("gramatica");
    ofstream fout;
    fout.open("gramTraduc");
    string cad;
    int cont=cantidadTokens+1;
    int z = cont;
    while(getline(fin,cad)){
        string aux="";
        for(int i=0;cad[i]!=' ';i++){
            aux+=cad[i];
        }
        if(!traductor[aux])
        traductor[aux]=cont++;
    }
    int extras = cont;
    fin.close();
    fin.open("gramatica");
    vector<bool> vistos(traductor.size()+1);
    vector<vector<int> > mapa(cantidadTokens+cont+5);
    while(getline(fin,cad)){
        fout<<z<<" ";
        int i = 0;
        while(cad[i]!=' ')i++;
        while(i<cad.size()){
            while(cad[i]!=cad.size() && cad[i]==' ')i++;
            if(i>=cad.size())continue;
            string aux = "";
            nodo_TRIE *pasito = arbol;
            while(i<cad.size() && cad[i]!=' '){
                aux+=cad[i];
                if(pasito!=nullptr)pasito = pasito->mov_seguro(cad[i]);
                i++;
            }
            if(aux == "|"){
                fout<<"\n";
                fout<<z<<" ";
                continue;
            }
            if(aux == "1d"){
                if(!traductor["1d"])traductor["1d"]=extras++;
                fout<<traductor["1d"]<<" ";
            }else if(aux == "3ps"){
                if(!traductor["3ps"])traductor["3ps"]=extras++;
                fout<<traductor["3ps"]<<" ";
            }else if(traductor[aux]){
                fout<<traductor[aux]<<" ";
            }else{
                if(pasito == nullptr){
                    cout<<"Error la palabra reservada "<<aux<<" no existe\n";
                    return false;
                }
                fout<<pasito->token<<" ";
            }
        }
        fout<<"\n";
        z++;
    }
    fout.close();
    return true;
}
#endif