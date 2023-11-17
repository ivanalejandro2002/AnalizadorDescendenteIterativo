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
    if(!registro(terminos,primeros,siguientes,arbol,cantidadTokens+1,todosComponentes,inversoterminos))return 0;
    //cout<<":)\n";
    /*for(int i=1;i<todosComponentes;i++){
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
    cout<<terminos.size()-cantidadTokens-1<<"\n";
    */
    vector<vector<vector<int> > > tabla(terminos.size()+1,vector<vector<int> >(cantidadTokens+4));
    int epsilon = cantidadTokens+1;
    if(!entabla(primeros,siguientes,tabla,cantidadTokens+2,terminos["3ps"],terminos["1d"]))return 0;
    /*for(int i =1;i<=cantidadTokens+1;i++){
        cout<<nombres[i]<<",";
    }
    cout<<"epsilon";
    cout<<"\n";
    for(int i =cantidadTokens+1;i<todosComponentes-2;i++){
        cout<<inversoterminos[i]<<"::\n";
        for(int j=1;j<=cantidadTokens+2;j++){
            cout<<"\t";
            if(j<=cantidadTokens+1)cout<<nombres[j]<<":";
            else cout<<"epsilon:";
            for(int z: tabla[i][j]){
                if(z<=cantidadTokens+1)cout<<nombres[z]<<",";
                else cout<<inversoterminos[z]<<",";
            }
            cout<<"\n";
        }
        cout<<"\n";
    }
    */
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
            //registro_Tokens.recorre();
        }
        if(!registro_Tokens.sintaxisea(tabla,0,registro_Tokens.indice_final,cantidadTokens+1,terminos["3ps"]))return 0;
        if(errores_general)cout<<"El programa termino con errores\n";
        else cout<<"El programa termino exitosamente\n";
    }else{
        string lectura;
        while(1){
            getline(cin,lectura);
            existe_Error = evalua(lectura,arbol,cantidadTokens,nombres,comentariote,registro_Tokens);
            if(existe_Error){
                while(registro_Tokens.indice_final>registro_Tokens.indice_inicial){
                    registro_Tokens.indice_final--;
                    registro_Tokens.elementos.pop_back();
                }
            }
            if(existe_Error)continue;
            //cout<<registro_Tokens.indice_inicial<<","<<registro_Tokens.indice_final<<"::\n";
            bool correcto;
            correcto = registro_Tokens.sintaxisea(tabla,registro_Tokens.indice_inicial,registro_Tokens.indice_final,cantidadTokens+1,terminos["3ps"]);
            
            if(!correcto){
                while(registro_Tokens.indice_final>registro_Tokens.indice_inicial){
                    registro_Tokens.indice_final--;
                    registro_Tokens.elementos.pop_back();
                }
                continue;
            }
            registro_Tokens.indice_inicial=registro_Tokens.indice_final;
            //registro_Tokens.recorre();
        }
    }
}