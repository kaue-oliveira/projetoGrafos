#include <iostream>
#include <fstream>
#include "grafo.h"
#include <string>
using namespace std;

int main() {
    // Criando um objeto da classe Grafo
    int numVertices = 100; // Supondo um número máximo de vértices
    Grafo grafo(numVertices);

    // Pergunta se o grafo é direcionado
    char resposta;
    cout << "O grafo é direcionado? (y/n): ";
    cin >> resposta;
    if (resposta == 'y' || resposta == 'Y') {
        grafo.direcionado = true;
    }

    // Lendo o grafo do arquivo
    grafo.lergrafo();

    // Imprimindo a matriz de adjacências
    grafo.imprimirmatriz();

    cout << "a implementação da lista ainda não esta funcionando =(" << endl;
    // Imprimindo a lista de vértices
    grafo.imprimirvertices();

    int grau =grafo.obtergrau("1");
    cout << grau << endl;
    return 0;
}