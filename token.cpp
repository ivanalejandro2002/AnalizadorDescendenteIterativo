#ifndef token_Cabecera
#define token_Cabecera
#include <iostream>
#include <vector>
using namespace std;
struct nodo_Token{
    int token;
    string tipo_Token;
    string lexema;
};
struct token{
    int indice_inicial,indice_final;
    vector<nodo_Token> elementos;
    token():indice_inicial(0),indice_final(0){};
    void agrega(int &numero_Token,string &tipoToken,string &lexema){
        elementos.push_back({numero_Token,tipoToken,lexema});
        indice_final++;
    }
    void recorre(){
        for(indice_inicial;indice_inicial<indice_final;indice_inicial++){
            cout<<elementos[indice_inicial].tipo_Token<<","<<elementos[indice_inicial].token<<"::"<<elementos[indice_inicial].lexema<<"\n";
        }
    }
    void elimina(){
        elementos.pop_back();
        indice_final--;
    }
};
#endif