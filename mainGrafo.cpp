#include <iostream>
#include <fstream>
#include "grafo.h"
using namespace std;
void exibirMenu() {
    cout << "====== Menu ======" << endl;
    cout << "1. Inserir vértice" << endl;
    cout << "2. Inserir aresta" << endl;
    cout << "3. Obter grau de um vértice" << endl;
    cout << "4. Imprimir matriz de adjacências" << endl;
    cout << "5. Imprimir lista de vértices" << endl;
    cout << "6. Remover vértice" << endl;
    cout << "7. Sair" << endl;
    cout << "===================" << endl;
}

int main() {
    int numVertices = 100; // Supondo um número máximo de vértices
    int opcao;
    Grafo grafo(numVertices);

    // Pergunta se o grafo é direcionado
    char resposta;
    cout << "O grafo é direcionado? (y/n): " << endl;
    cout << "atenção isso pode causar mudanças no arquivo lido:" ;
    cin >> resposta;
    if (resposta == 'y' || resposta == 'Y') {
        grafo.direcionado = true;
    }
    grafo.lergrafo();
    grafo.reescreverArquivo();

    do {
        exibirMenu();
        cout << "Escolha uma opção: ";
        cin >> opcao;

        switch (opcao) {
            case 1: {
                string vertice;
                cout << "Digite o nome do vértice a ser inserido: ";
                cin >> vertice;
                grafo.inserevertice(vertice);
                grafo.reescreverArquivo();
                break;
            }
            case 2: {
                string saida, entrada;
                cout << "Digite o nome do vértice de saída: ";
                cin >> saida;
                cout << "Digite o nome do vértice de entrada: ";
                cin >> entrada;
                grafo.inserearesta(saida, entrada);
                grafo.reescreverArquivo();
                break;
            }
            case 3: {
                string vertice;
                cout << "Digite o nome do vértice para obter o grau: ";
                cin >> vertice;
                cout << "Grau do vértice: " << grafo.obtergrau(vertice) << endl;
                break;
            }
            case 4:
                grafo.imprimirmatriz();
                break;
            case 5:
                grafo.imprimirvertices();
                break;
            case 6: {
                string verticeRemover;
                cout << "Digite o nome do vértice a ser removido: ";
                cin >> verticeRemover;
                grafo.removervertice(verticeRemover);
                grafo.reescreverArquivo();
                break;
            }
            case 7:
                cout << "Saindo..." << endl;
                break;
            default:
                cout << "Opção inválida!" << endl;
        }
    } while (opcao != 7);
    
    grafo.reescreverArquivo();
    return 0;
}