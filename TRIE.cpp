#ifndef TRIE_Cabecera
#define TRIE_Cabecera
#include <iostream>
#include <vector>

using namespace std;
struct nodo_TRIE;
struct lista{
    int valor;
    lista *siguiente;
    lista(char v):valor(v),siguiente(nullptr){};
};
struct nodo_TRIE{
    int trabado;
    int token;
    lista *elementos;
    lista *ultimo;
    vector<nodo_TRIE*> hijos;
    nodo_TRIE():token(0),elementos(new lista(1)),ultimo(elementos),trabado(0),hijos(vector<nodo_TRIE*>(256,nullptr)){};
    int asigna_Token(int valor_Token){
        if(this -> token == 0)this -> token = valor_Token;
        return this -> token;
    }
    void ingresa_token(string &cadena, int pos, int &cantidadTokens){
        if(pos==cadena.size()){
            if(token){
                cantidadTokens--;
                return;
            }
            token = cantidadTokens;
            return;
        }
        pasito(cadena[pos])->ingresa_token(cadena,pos+1,cantidadTokens);
    }
    void dfs(vector<char> &construyo){
        if(token){
            cout<<token<<",";
            for(char co: construyo)cout<<co;
            cout<<"\n";
        }
        lista *it = elementos;
        while(it!=nullptr){
            it = it->siguiente;
            if(it==nullptr)continue;
            construyo.push_back(it->valor);
            hijos[(int)it->valor]->dfs(construyo);
        }
        construyo.pop_back();
    }
    void saca_Tokens(){
        vector<char> construyo;
        this->dfs(construyo);
    }
    void inicializa(int &cantidadTokens, vector<string> &tokens){
        for(string lectura: tokens)
            this->ingresa_token(lectura,0,++cantidadTokens);
    }
    nodo_TRIE *pasito(char letra){
        if(hijos[(int)letra]==nullptr){
            hijos[(int)letra]=new nodo_TRIE();
            ultimo->siguiente=new lista(letra);
            ultimo = ultimo->siguiente;
        }
        return this->hijos[(int)letra];
    }
    nodo_TRIE *mov_seguro(char letra){
        return this->hijos[(int)letra];
    }
};
#endif