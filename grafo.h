// Arquivo grafo.h
#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Grafo {
public:
    int arestanula;
    int maxvertices;
    int numvertices;
    string* vertices;
    int** matrizadjacencias;
    bool direcionado; // Indica se o grafo é direcionado ou não


    Grafo(int max, int valorarestanula);
    ~Grafo();
    int obterindice(string item);
    void inserevertice(string item);
    void inserearesta(string Nosaida, string Noentrada, int peso);
    int obtergrau(string item);
    void imprimirmatriz();
    void imprimirvertices();
    void lergrafo();
};

#endif // GRAFO_H
