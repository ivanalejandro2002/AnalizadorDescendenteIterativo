#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <fstream>
#include <set>
#include "TRIE.cpp"
#include "automata.cpp"
#include "token.cpp"
#include "sintaxis.cpp"
using namespace std;

int main(int argc,char *argv[]){
    nodo_TRIE *arbol = new nodo_TRIE();
    token registro_Tokens = token();
    nodo_TRIE *it = arbol;
    int cantidadTokens = 0;
    vector< string > reservadas = {"select", "distinct", "from"};
    vector<string> simbolos = {"*",",","."};
    vector<string> nombres = {"","select","distinct","from","star","coma","dot"};
    nombres.push_back("NUMERO");
    nombres.push_back("IDENTIFICADOR");
    nombres.push_back("CADENA");
    arbol->inicializa(cantidadTokens , reservadas);
    arbol -> inicializa(cantidadTokens , simbolos);
    /*for(int i = 1; i < argc ; i++){
        cout << argv[i] << "\n";
    }*/
    arbol->pasito('-')->pasito('-')->trabado=1;
    arbol->pasito('/')->pasito('*')->trabado=2;
    arbol->pasito('*')->pasito('/')->trabado=3;
    cantidadTokens++;
    nodo_TRIE actual;
    //arbol -> saca_Tokens();
    //------------------Chequeo de sintáxis----------------------------------------
    map<string,int> terminos;
    map<int,string> inversoterminos;
    vector<set<int> > primeros;
    vector<set<int> > siguientes;
    int todosComponentes=0;
    registro(terminos,primeros,siguientes,arbol,cantidadTokens+1,todosComponentes,inversoterminos);
    //cout<<":)\n";
    for(int i=1;i<todosComponentes;i++){
        cout<<inversoterminos[i]<<"::\n{";
        for(int z:primeros[i]){
            if(z>cantidadTokens+1){cout<<"Epsilon, ";continue;}
            cout<<nombres[z]<<", ";
        }
        cout<<"}\n";
    }
    cout<<"\n\n";
    for(int i=1;i<todosComponentes;i++){
        cout<<inversoterminos[i]<<"::\n{";
        for(int z:siguientes[i]){
            if(z>cantidadTokens+1){cout<<"Epsilon, ";continue;}
            cout<<nombres[z]<<", ";
        }
        cout<<"}\n";
    }
    //-----------------------------------------------------------------------------
    bool existe_Error=0;
    bool comentariote = 0;
    ifstream fin;
    fin.open(argv[1]);
    if(argc>1){
        bool errores_general=0;
        string lecturas;
        while(getline(fin,lecturas)){
            existe_Error = evalua(lecturas,arbol,cantidadTokens,nombres,comentariote,registro_Tokens);
            errores_general|=existe_Error;
            if(existe_Error)continue;
            registro_Tokens.recorre();
        }
        if(errores_general)cout<<"El programa termino con errores\n";
        else cout<<"El programa termino exitosamente\n";
    }else{
        string lectura;
        while(1){
            getline(cin,lectura);
            existe_Error = evalua(lectura,arbol,cantidadTokens,nombres,comentariote,registro_Tokens);
            if(existe_Error)continue;
            registro_Tokens.recorre();
        }
    }
}