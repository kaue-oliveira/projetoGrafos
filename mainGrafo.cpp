#include <iostream>
#include <fstream>
#include <unistd.h> // Para o usleep
#include "grafo.h"
#include "filadinamica.h"
#include "pilhadinamica.h"

using namespace std;

void exibirMenu() {
    cout << "\033[2J\033[1;1H"; 
    cout << "\033[1m\033[33m"; 
    cout << "====================================================" << endl;
    cout << "|                     M E N U                      |" << endl;
    cout << "====================================================" << endl;
    cout << "\033[0m";
    cout << "\033[1m\033[34m";
    cout << "1. Inserir vértice" << endl;
    cout << "2. Inserir aresta" << endl;
    cout << "3. Obter grau de um vértice" << endl;
    cout << "4. Imprimir matriz de adjacências" << endl;
    cout << "5. Imprimir lista de vértices" << endl;
    cout << "6. Remover vértice" << endl;
    cout << "7. Numero de vertices" << endl;
    cout << "8. Numero de arestas" << endl;
    cout << "9. Busca em largura" << endl;
    cout << "10. Busca em profundidade" << endl;
    cout << "11. Sair" << endl;
    cout << "\033[0m"; 
    cout << "\033[1m\033[33m";
    cout << "====================================================" << endl;
    cout << "\033[0m";
    cout << "Escolha uma opção: ";
}

void limpaTela() {
    cout << "\033[2J\033[1;1H";
}

void animacaoTransicao() {
    limpaTela();
    cout << "\033[1m\033[32m";
    cout << "====================================================" << endl;
    cout << "|                   Carregando...                   |" << endl;
    cout << "====================================================" << endl;
    cout << "\033[0m";
    usleep(1000000); // 1 segundo
}

int main() {
    int numVertices = 100; // Supondo um número máximo de vértices
    int opcao;
    string item1, item2;
    Grafo grafo(numVertices);

    
    char resposta;
    cout << "O grafo é direcionado? (y/n): ";
    cin >> resposta;
    if (resposta == 'y' || resposta == 'Y') {
        grafo.direcionado = true;
    }
    grafo.lergrafo();
    grafo.reescreverArquivo();

    do {
        limpaTela();
        exibirMenu();
        cin >> opcao;

        switch (opcao) {
            case 1: {
                animacaoTransicao();
                string vertice;
                cout << "Digite o nome do vértice a ser inserido: ";
                cin >> vertice;
                grafo.inserevertice(vertice);
                grafo.reescreverArquivo();
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 2: {
                animacaoTransicao();
                string saida, entrada;
                cout << "Digite o nome do vértice de saída: ";
                cin >> saida;
                cout << "Digite o nome do vértice de entrada: ";
                cin >> entrada;
                grafo.inserearesta(saida, entrada);
                grafo.reescreverArquivo();
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 3: {
                animacaoTransicao();
                string vertice;
                cout << "Digite o nome do vértice para obter o grau: ";
                cin >> vertice;
                cout << "Grau do vértice: " << grafo.obtergrau(vertice) << endl;
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 4: {
                animacaoTransicao();
                grafo.imprimirmatriz();
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 5: {
                animacaoTransicao();
                grafo.imprimirvertices();
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 6: {
                animacaoTransicao();
                string verticeRemover;
                cout << "Digite o nome do vértice a ser removido: ";
                cin >> verticeRemover;
                grafo.removervertice(verticeRemover);
                grafo.reescreverArquivo();
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 7: {
                animacaoTransicao();
                cout << "A quantidade de vertices no grafo é: ";
                cout << grafo.qtdvertice() << endl;
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 8: {
                animacaoTransicao();
                cout << "A quantidade de arestas no grafo é: ";
                cout << grafo.qtdarestas() << endl;
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 9: {
                animacaoTransicao();
                cout << "Digite o vertice de origem: ";
                cin >> item1;
                cout << "Digite o vertice de destino: ";
                cin >> item2;
                grafo.buscaemlargura(item1, item2);
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 10: {
                animacaoTransicao();
                cout << "Digite o vertice de origem: ";
                cin >> item1;
                cout << "Digite o vertice de destino: ";
                cin >> item2;
                grafo.buscaemprofundidade(item1, item2);
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 11:
                cout << "Obrigado por usar o nosso sitema =)" << endl;
                break;
            default:
                cout << "Opção inválida!" << endl;
        }
        usleep(500000); // Aguarda 5.5 segundos para dar tempo de ler antes de limpar a tela
    } while (opcao != 11);
    
    grafo.reescreverArquivo();
    return 0;
}
