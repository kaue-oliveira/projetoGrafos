#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));

    ofstream arq;
    arq.open("grafo.txt");

    // Escreve os vértices
    arq << "V = {";
    for(int i = 0; i < 10000; i++) {
        arq << i;
        if (i < 9999) {
            arq << ",";
        }
    }
    arq << "};";

    // Escreve as arestas
    arq << " A = {";
    for (int i = 0; i < 9999; i++) {
        arq << "(" << i << "," << i+1 << "," << rand() % 21 - 10 << "),"; // Aresta para o próximo vértice
    }
    // Conecta o último vértice ao primeiro
    arq << "(" << 9999 << "," << 0 << "," << rand() % 21 - 10 << ")"; // Última aresta fechando o ciclo
    arq << "};";

    arq.close();

    return 0;
}
