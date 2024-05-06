#include <iostream>
#include <fstream>
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

    grafo.lergrafo();

    grafo.imprimirmatriz();

    
    
    grafo.imprimirvertices();

    int grau =grafo.obtergrau("2");
    cout << "Grau do vertice 2 = " << grau << endl;



    try {
      string verticeRemover;
      cout << "insira um vertice pra remover: " << endl;
      cin >> verticeRemover;
      grafo.removervertice(verticeRemover);
    }
    catch (const std::exception& e) {
        cerr << "Erro: " << e.what() << endl;
    }
    
    
    grafo.imprimirmatriz();

    
    
    
    
    return 0;
}
