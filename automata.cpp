#include<iostream>
#include<vector>
#include<ctype.h>
#include "TRIE.cpp"
#include "token.cpp"

using namespace std;

bool evalua(string &mens, nodo_TRIE *arbol,int cantidadTokens, vector<string> &nombres,bool &comentariote,token &registro_Tokens){
    nodo_TRIE *it;
    int tamano_actual = registro_Tokens.indice_final;
    for(int i = 0; i < mens.size() ; i++){
        string lexema = "";
        if(comentariote){
            if(i<mens.size()-1 && mens[i]=='*' && mens[i+1]=='/'){
                comentariote=0;
                i++;
            }
            continue;
        }
        char caracter = mens[i];
        if(caracter==' ')continue;
        if(isalpha(caracter)){
            it = arbol;
            while( i < mens.size() && (isalpha(caracter) || isdigit(caracter))){
                lexema+=caracter;
                i++;
                it = it -> pasito(caracter);
                caracter = mens[i];
            }
            //cout<<":: Numero de Token: "<<it->asigna_Token(cantidadTokens+1)<<"; Tipo de Token: "<<nombres[it->token]<<","<<lexema<<"\n";
            it->asigna_Token(cantidadTokens+1);
            registro_Tokens.agrega(it->token,nombres[it->token],lexema);
            i--;
        }else if(isdigit(caracter)){
            it = arbol;
            while(i< mens.size() && isdigit(caracter) ){
                lexema+=caracter;
                i++;
                it = it->pasito(caracter);
                caracter = mens[i];
            }
            if(i>=mens.size()){
                //cout<<":: Numero de Token: "<<it->asigna_Token(cantidadTokens)<<"; Tipo de Token: "<<nombres[it->token]<<","<<lexema<<"\n";
                it->asigna_Token(cantidadTokens);
                registro_Tokens.agrega(it->token,nombres[it->token],lexema);
                continue;
            }
            if(caracter!='.' && caracter!='E'){
                i--;
                //cout<<":: Numero de Token: "<<it->asigna_Token(cantidadTokens)<<"; Tipo de Token: "<<nombres[it->token]<<","<<lexema<<"\n";
                it->asigna_Token(cantidadTokens);
                registro_Tokens.agrega(it->token,nombres[it->token],lexema);
                continue;
            }
            if(caracter=='.'){
                lexema+=caracter;
                it = it->pasito(caracter);
                i++;
                caracter = mens[i];
                if(i>=mens.size() || !isdigit(caracter)){
                    cout<<"Error: Los numeros deben tener fomrato D.DESD\nD=Digitos\nS=signo\n";
                    while(registro_Tokens.indice_final>tamano_actual)registro_Tokens.elimina();
                    return 1;
                }
                while(i< mens.size() && isdigit(caracter)){
                    lexema+=caracter;
                    i++;
                    it = it->pasito(caracter);
                    caracter = mens[i];
                }
            }

            if(i>=mens.size()){
                //cout<<":: Numero de Token: "<<it->asigna_Token(cantidadTokens)<<"; Tipo de Token: "<<nombres[it->token]<<","<<lexema<<"\n";
                it->asigna_Token(cantidadTokens);
                registro_Tokens.agrega(it->token,nombres[it->token],lexema);
                continue;
            }
            if(caracter!='E'){
                i--;
                //cout<<":: Numero de Token: "<<it->asigna_Token(cantidadTokens)<<"; Tipo de Token: "<<nombres[it->token]<<","<<lexema<<"\n";
                it->asigna_Token(cantidadTokens);
                registro_Tokens.agrega(it->token,nombres[it->token],lexema);
                continue;
            }
            lexema+=caracter;
            it = it->pasito(caracter);
            i++;
            if(i>=mens.size()){
                cout<<":::Error: Los numeros deben tener fomrato D.DESD\nD=Digitos\nS=signo\n";
                while(registro_Tokens.indice_final>tamano_actual)registro_Tokens.elimina();
                return 1;
            }
            caracter = mens[i];
            if(caracter!='+' && caracter!='-' && caracter<'0' && caracter>'9'){
                i--;
                //cout<<":: Numero de Token: "<<it->asigna_Token(cantidadTokens)<<"; Tipo de Token: "<<nombres[it->token]<<","<<lexema<<"\n";
                it->asigna_Token(cantidadTokens);
                registro_Tokens.agrega(it->token,nombres[it->token],lexema);
                continue;
            }
            if(caracter=='+' || caracter == '-'){
                lexema+=caracter;
                it = it->pasito(caracter);
                i++;
                caracter = mens[i];
                if(i>=mens.size() || !isdigit(caracter)){
                    cout<<":::Error: Los numeros deben tener fomrato D.DESD\nD=Digitos\nS=signo\n";
                    while(registro_Tokens.indice_final>tamano_actual)registro_Tokens.elimina();
                    return 1;
                }
            }
            while(i< mens.size() && isdigit(caracter)){
                lexema+=caracter;
                i++;
                it = it->pasito(caracter);
                caracter = mens[i];
            }
            //cout<<":: Numero de Token: "<<it->asigna_Token(cantidadTokens)<<"; Tipo de Token: "<<nombres[it->token]<<","<<lexema<<"\n";
            it->asigna_Token(cantidadTokens);
            registro_Tokens.agrega(it->token,nombres[it->token],lexema);
            if(i>=mens.size())continue;
            i--;
            continue;
        }else if(caracter=='"'){
            it = arbol;
            i++;
            caracter = mens[i];
            while(i<mens.size() && caracter!='"'){
                lexema+=caracter;
                it->pasito(caracter);
                i++;
                caracter = mens[i];
            }
            if(caracter != '"'){
                cout<<"::Error se debe de terminar la cadena en la misma linea\n";
                while(registro_Tokens.indice_final>tamano_actual)registro_Tokens.elimina();
                return 1;
            }
            //cout<<":: Numero de Token: "<<it->asigna_Token(cantidadTokens+2)<<"; Tipo de Token: "<<nombres[it->token]<<","<<lexema<<"\n";
            it->asigna_Token(cantidadTokens+2);
            registro_Tokens.agrega(it->token,nombres[it->token],lexema);
        }else{
            it = arbol;
            if(it->mov_seguro(caracter)==nullptr){
                cout<<"Error:. No existe un operador que inicie con "<<caracter<<"\n";
                while(registro_Tokens.indice_final>tamano_actual)registro_Tokens.elimina();
                return 1;
            }
            string paso = "";
            while(i<mens.size() && it->mov_seguro(caracter)!=nullptr){
                paso+=caracter;
                it = it->mov_seguro(caracter);
                i++;
                caracter = mens[i];
                if(it->trabado==1)return 0;
                if(it->trabado==2){
                    comentariote=1;
                    break;
                }
                if(it->trabado==3){
                    cout<<"::Error Para un comentario grande, debes de colocar /*comentario multilinea*/\n";
                    while(registro_Tokens.indice_final>tamano_actual)registro_Tokens.elimina();
                    return 1;
                }
            }
            if(comentariote)continue;
            //cout<<paso<<"::Numero de Token "<<it->token<<"; Tipo de Token: "<<nombres[it->token]<<","<<lexema<<"\n";
            registro_Tokens.agrega(it->token,nombres[it->token],paso);
            if(i<mens.size())i--;
        }
    }
    return 0;
}
bool evalua(char *mens,nodo_TRIE *arbol,int cantidadTokens, vector<string> &nombres,bool &comentariote, token &registro_Tokens){
    int i=0;
    string traductor = "";
    while(mens[i] != '\0' || mens[i]=='"')traductor += mens[i++];
    cout<<traductor<<"\n";
    return evalua(traductor,arbol,cantidadTokens,nombres,comentariote,registro_Tokens);
}