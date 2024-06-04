// Arquivo grafo.h
#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "filadinamica.h"
#include "pilhadinamica.h"
using namespace std;

class Grafo {
public:
    int arestanula;
    int maxvertices;
    int numvertices;
    string* vertices;
    int** matrizadjacencias;
    bool direcionado; // Indica se o grafo é direcionado ou não
    bool* marcador;
    bool valorado;


    Grafo(int max, int nulo);
    ~Grafo();
    int obterindice(string item);
    void inserevertice(string item);
    void inserearesta(string Nosaida, string Noentrada);
    void obtergrau(string item);
    void imprimirmatriz();
    void imprimirvertices();
    void lergrafo();
    void reescreverArquivo();
    void removeraresta(string origem, string destino);
    void removervertice(string verticeEditar);
    int qtdvertice();
    int qtdarestas();
    void limpamarcador();
    void buscaemlarguraArvore();
    void buscaemprofundidadeArvore();
    void buscaemprofundidade(string origem, string destino);
    void buscaemlargura(string origem, string destino);
    bool ehConexo();
    void possuiciclos();
    void eheuleriano();
    int obtergrauaux(string item);
    bool ehfortementeconexo();
    void verificaExistencia(string vertice);
    void verificaExistenciaAresta(string origem, string entrada, string peso);
    void prim();
    void inserearestaValorado(string Nosaida, string Noentrada, string peso); //caso seja valorado
    bool verifica(const string& linha);
    
    
};

#endif // GRAFO.H       


