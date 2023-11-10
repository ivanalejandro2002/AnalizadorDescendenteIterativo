#ifndef sintaxis_Cabecera
#define sintaxis_Cabecera
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <stack>
using namespace std;
struct nodoCiclo{
    int grupos;
    vector<int> cuentas;
    bool epsilon;
    void inicializa(){
        cuentas.resize(grupos+1);
    }
};
struct aristaCiclo{
    int destino;
    int grupo;
    int prioridad;
};
int act= 0;
bool tarjan(int nodo,vector<vector<aristaCiclo> > &mapa,vector<nodoCiclo> &infoNodos,vector<int> &vis,vector<pair<int,int> > &visitas,vector<int> &dag,stack<int> &pila,int &cont,int cantidadTokens,int codeps){
    if(vis[nodo] > 0)return infoNodos[nodo].epsilon;
    vis[nodo] = 1;
    visitas[nodo].first = visitas[nodo].second=++act;
    pila.push(nodo);

    for(auto z:mapa[nodo]){
        /*if(nodo ==9){
            cout<<nodo<<","<<z.destino<<","<<infoNodos[nodo].cuentas[z.grupo]<<","<<z.prioridad<<"\n";
        }*/
        if(infoNodos[nodo].cuentas[z.grupo]<z.prioridad)continue;
        //if(nodo == 9)cout<<"O\n";
        if(!vis[z.destino]){
            bool x = tarjan(z.destino,mapa,infoNodos,vis,visitas,dag,pila,cont,cantidadTokens,codeps);
            infoNodos[nodo].epsilon|=x;
            if(x  && z.destino>cantidadTokens)infoNodos[nodo].cuentas[z.grupo]++;
            visitas[nodo].second = min(visitas[nodo].second,visitas[z.destino].second);
        }else{
            if(infoNodos[z.destino].epsilon  && z.destino>cantidadTokens)infoNodos[nodo].cuentas[z.grupo]++;
            infoNodos[nodo].epsilon|=infoNodos[z.destino].epsilon;
            if(vis[z.destino]==1){
                visitas[nodo].second = min(visitas[nodo].second,visitas[z.destino].first);
            }
        }
    }

    int k = 0;
    if(visitas[nodo].first == visitas[nodo].second){
        cont++;
        int zi =0;
        do{
            k = pila.top();
            pila.pop();
            dag[k] = cont;
            vis[k] = 2;
        }while(k!=nodo);
    }
    return infoNodos[nodo].epsilon;
}

void busca(int nodo, vector<set<int> > &primeros, vector<vector<int> > &mapa,vector<int> &vis,int cantidadTokens,int g1,int g2){
    if(vis[nodo])return;
    vis[nodo]=1;
    for(int z:mapa[nodo]){
        busca(z,primeros,mapa,vis,cantidadTokens,g1,g2);
        for(int w:primeros[z])if(w<=cantidadTokens || w==g1 || w==g2)primeros[nodo].insert(w);
    }
}

bool registro(map<string,int> &traductor,vector<set<int> > &primeros,nodo_TRIE *arbol,int cantidadTokens,int &todosComponentes,map<int,string> &inversoterminos){
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
        if(!traductor[aux]){
            traductor[aux]=cont++;
            inversoterminos[cont-1]=aux;
        }
    }
    int extras = cont;
    fin.close();
    fin.open("gramatica");
    vector<bool> vistos(traductor.size()+1);
    primeros.resize(cantidadTokens+cont+5);
    vector<vector<aristaCiclo> > mapa(cantidadTokens+cont+5);
    vector<nodoCiclo> infoNodos(cantidadTokens+cont+5);
    vector<int> vis(cantidadTokens+cont+5);
    vector<pair<int,int> > visitas(cantidadTokens+cont+5);
    vector<int> dag(cantidadTokens+cont+5);
    stack<int> pila;
    int componentes = 0;
    while(getline(fin,cad)){
        vector<int> elementos;
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
                fout<<elementos.size()<<" ";
                for(int z: elementos)fout<<z<<" ";
                elementos.clear();
                fout<<"\n";
                fout<<z<<" ";
                continue;
            }
            if(aux == "1d"){
                if(!traductor["1d"]){
                    traductor["1d"]=extras++;
                    inversoterminos[extras-1]="1d";
                }
                elementos.push_back(traductor["1d"]);
            }else if(aux == "3ps"){
                if(!traductor["3ps"]){
                    traductor["3ps"]=extras++;
                    inversoterminos[extras-1]="3ps";
                }
                elementos.push_back(traductor["3ps"]);
            }else if(traductor[aux]){
                elementos.push_back(traductor[aux]);
            }else{
                if(pasito == nullptr){
                    cout<<"Error la palabra reservada "<<aux<<" no existe\n";
                    return false;
                }
                elementos.push_back(pasito->token);
            }
        }
        fout<<elementos.size()<<" ";
        for(int z:elementos)fout<<z<<" ";
        fout<<"\n";
        z++;
    }
    fout.close();
    fin.close();
    fin.open("gramTraduc");
    int origen,siguiente,elementos;
    int ant = -1;
    int rep = 0;
    while(fin>>origen){
        if(origen!=ant){
            rep = 0;
            ant = origen;
        }
        rep++;
        infoNodos[origen].grupos++;
        fin>>elementos;
        for(int i =0;i<elementos;i++){
            fin>>siguiente;
            mapa[origen].push_back({siguiente,infoNodos[origen].grupos,i});
        }
    }

    for(int i =cantidadTokens+1;i<cont;i++)
        infoNodos[i].inicializa();

    infoNodos[traductor["3ps"]].epsilon = 1;
    for(int i =1;i<extras;i++)
        tarjan(i,mapa,infoNodos,vis,visitas,dag,pila,componentes,cantidadTokens,traductor["3ps"]);
    
    for(int i =1;i<extras;i++){
        cout<<i<<","<<dag[i]<<"::<<"<<infoNodos[i].epsilon<<"\n";
    }

    set<pair<int,int> > listo;
    vector<vector<int> > dagueado(componentes+2);
    for(int i =1;i<extras;i++){
        for(auto z: mapa[i]){
            if(dag[i]!=dag[z.destino]){
                if(infoNodos[i].cuentas[z.grupo]>=z.prioridad && !listo.count({dag[i],dag[z.destino]})){
                    dagueado[dag[i]].push_back(dag[z.destino]);
                    listo.insert({dag[i],dag[z.destino]});
                }
            }
        }
    }
    vector<set<int> >primerosdag(componentes+2);
    vis.clear();
    vis.resize(componentes+2);
    for(int i =1;i<=cantidadTokens;i++){
        primerosdag[i].insert(i);
    }
    primerosdag[dag[traductor["3ps"]]].insert(traductor["3ps"]);
    primerosdag[dag[traductor["1d"]]].insert(cantidadTokens);
    for(int i=1;i<=componentes;i++){
        busca(i,primerosdag,dagueado,vis,cantidadTokens,traductor["3ps"],traductor["1d"]);
    }
    vector<vector<int> > tradag(componentes+2);
    for(int i=1;i<extras;i++){
        tradag[dag[i]].push_back(i);
    }
    cout<<":0";
    for(int i=1;i<=componentes;i++){
        for(int z:tradag[i]){
            for(int zi: primerosdag[i]){
                if(zi==traductor["3ps"])continue;
                primeros[z].insert(zi);
            }
            if(infoNodos[z].epsilon)primeros[z].insert(traductor["3ps"]);
        }
    }
    /*for(int i=1;i<=componentes;i++){
        cout<<i<<":: ";
        for(int z:primerosdag[i])cout<<z<<",";
        cout<<"\n";
    }
    for(int i =1;i<=componentes;i++){
        cout<<i<<"|||";
        for(int z:dagueado[i])cout<<z<<",";
        cout<<"\n";
    }
    for(int i=cantidadTokens+1;i<cont;i++){
        cout<<i<<"__";
        for(int z:infoNodos[i].cuentas)cout<<z<<",";
        cout<<"\n";
    }*/
    todosComponentes = extras;
    return true;
}
#endif