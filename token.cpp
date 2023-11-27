#ifndef token_Cabecera
#define token_Cabecera
#include <iostream>
#include <vector>
#include <stack>
#include <map>
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
    bool sintaxisea(vector<vector<vector<int> > > &tabla,int inicio, int fin,int cantidadTokens,int vacio,map<int,string> &inversoterminos, vector<string> &nombres){
        stack<int> pila;
        pila.push(cantidadTokens+1);
        for(int i = inicio;i<fin;){
            int posactual = pila.top();
            pila.pop();
            if(posactual>cantidadTokens && posactual!=vacio){
                nodo_Token z = elementos[i];
                if(!tabla[posactual][z.token].size()){
                    cout<<"No se esperaba el termino "<<nombres[z.token]<<" ";
                    if(z.token==cantidadTokens)cout<<z.lexema;
                    cout<<" en el elemento "<<i-inicio+1<<"\n";
                    return 0;
                }
                for(int i = tabla[posactual][z.token].size()-1;i>=0;i--){
                    //cout<<tabla[posactual][z.token][i]<<" ";
                    pila.push(tabla[posactual][z.token][i]);
                }
                //cout<<"\n";
            }else{
                if(posactual==vacio){
                    continue;
                }
                nodo_Token z = elementos[i];
                if(posactual!=z.token){
                    cout<<"Se esperaba el token '"<<nombres[posactual]<<"' En el elemento "<<i-inicio+1<<", se encontro: "<< nombres[z.token];
                    if(z.token==cantidadTokens)cout<<" "<<z.lexema;
                    cout<<"\n";
                    return 0;
                }
                i++;
            }
        }
        while(!pila.empty()){
            int posactual = pila.top();
            pila.pop();
            if(posactual>cantidadTokens && posactual!=vacio){
                int ztoken = cantidadTokens+1;
                //cout<<ztoken<<"\n";
                if(!tabla[posactual][ztoken].size()){
                    cout<<"La instruccion esta incompleta\n";
                    return 0;
                }else{
                    for(int i = tabla[posactual][ztoken].size()-1;i>=0;i--){
                        //cout<<tabla[posactual][ztoken][i];
                        pila.push(tabla[posactual][ztoken][i]);
                    }
                    //cout<<"\n";
                }
            }else{
                if(posactual!=vacio){
                    cout<<"La instruccion esta incompleta II\n";
                    return 0;
                }
            }
        }
        return 1;
    }
};
#endif