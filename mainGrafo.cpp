#include <iostream>
#include "grafo.h"
using namespace std;

int main() {
    int numVertices = 100; // Supondo um número máximo de vértices
    Grafo grafo(numVertices);

    // Pergunta se o grafo é direcionado
    char resposta;
    cout << "O grafo é direcionado? (y/n): ";
    cin >> resposta;
    if (resposta == 'y' || resposta == 'Y') {
        grafo.direcionado = true;
    }


    grafo.imprimirmatriz();

    
    
    grafo.imprimirvertices();

    int grau =grafo.obtergrau("1");
    cout << "Grau do vertice 1 = " << grau << endl;
    return 0;

}


