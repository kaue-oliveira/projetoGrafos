#include <iostream>
#include <fstream>
#include <unistd.h> // Para o usleep
#include "grafo.h"
#include "filadinamica.h"
#include "pilhadinamica.h"

using namespace std;
void exibirMenuDirecionado() {
    cout << "\033[2J\033[1;1H"; 
    cout << "\033[1m\033[33m"; 
    cout << "====================================================" << endl;
    cout << "|                     M E N U                      |" << endl;
    cout << "====================================================" << endl;
    cout << "\033[0m";
    cout << "\033[1m\033[34m";
    cout << "| 1. Inserir vértice                               |" << endl;
    cout << "| 2. Inserir aresta                                |" << endl;
    cout << "| 3. Obter grau de um vértice                      |" << endl;
    cout << "| 4. Imprimir matriz de adjacências                |" << endl;
    cout << "| 5. Imprimir lista de vértices                    |" << endl;
    cout << "| 6. Remover vértice                               |" << endl;
    cout << "| 7. Numero de vertices                            |" << endl;
    cout << "| 8. Numero de arestas                             |" << endl;
    cout << "| 9. Busca em largura                              |" << endl;
    cout << "| 10. Busca em profundidade                        |" << endl;
    cout << "| 11. Remover aresta                               |" << endl;
    cout << "| 12. Verificar se é conexo                        |" << endl;
    cout << "| 13. Verificar quantidade de ciclos               |" << endl;
    cout << "| 14. Verificar se é fortemente conexo             |" << endl;
    cout << "| 15. Verificar se grafo é euleriano               |" << endl;
    cout << "| 16. Algoritmo de prim                            |" << endl;
    cout << "| 17. Arvore busca em largura                      |" << endl;
    cout << "| 18. Arvore busca em profundidade                 |" << endl;
    cout << "| 19. Sair                                         |" << endl;
    cout << "\033[0m"; 
    cout << "\033[1m\033[33m";
    cout << "====================================================" << endl;
    cout << "\033[0m";
    cout << "Escolha uma opção: ";
}

void exibirMenu() {
    cout << "\033[2J\033[1;1H"; 
    cout << "\033[1m\033[33m"; 
    cout << "====================================================" << endl;
    cout << "|                     M E N U                      |" << endl;
    cout << "====================================================" << endl;
    cout << "\033[0m";
    cout << "\033[1m\033[34m";
    cout << "| 1. Inserir vértice                               |" << endl;
    cout << "| 2. Inserir aresta                                |" << endl;
    cout << "| 3. Obter grau de um vértice                      |" << endl;
    cout << "| 4. Imprimir matriz de adjacências                |" << endl;
    cout << "| 5. Imprimir lista de vértices                    |" << endl;
    cout << "| 6. Remover vértice                               |" << endl;
    cout << "| 7. Numero de vertices                            |" << endl;
    cout << "| 8. Numero de arestas                             |" << endl;
    cout << "| 9. Busca em largura                              |" << endl;
    cout << "| 10. Busca em profundidade                        |" << endl;
    cout << "| 11. Remover aresta                               |" << endl;
    cout << "| 12. Verificar se é conexo                        |" << endl;
    cout << "| 13. Verificar quantidade de ciclos               |" << endl;
    cout << "| 14. Verificar se grafo é euleriano               |" << endl;
    cout << "| 15. Algoritmo de prim                            |" << endl;
    cout << "| 16. Arvore busca em largura                      |" << endl;
    cout << "| 17. Arvore busca em profundidade                 |" << endl;
    cout << "| 18. Sair                                         |" << endl;
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
    usleep(05000000); // 1 segundo
}
bool verifica(const string& line) {
    size_t vertices_pos = line.find("V = {");
    size_t arestas_pos = line.find("}; A = {");

    if (vertices_pos == string::npos || arestas_pos == string::npos) {
        cerr << "Erro de formatação: falta {} encerrando os vértices ou arestas." << endl
             << "Por favor, garanta que o formato no arquivo está no seguinte padrão: V = {a,b,c,...}; A = {(a,b),(b,c),...}; ou V = {a,b,c,...}; A = {(a,b,1),(b,c,-2),...};" << endl;
        exit(EXIT_FAILURE);
    }

    size_t pos = arestas_pos + 6;
    size_t end_pos = line.find("};", pos);
    if (end_pos == string::npos) {
        cerr << "Erro de formatação: falta '};' encerrando as arestas." << endl;
        exit(EXIT_FAILURE);
    }

    string arestas_str = line.substr(pos, end_pos - pos);

    size_t open_paren_pos = arestas_str.find('(');
    while (open_paren_pos != string::npos) {
        size_t close_paren_pos = arestas_str.find(')', open_paren_pos);
        if (close_paren_pos == string::npos) {
            cerr << "Erro de formatação: falta ')' encerrando uma aresta." << endl;
            exit(EXIT_FAILURE);
        }

        string aresta = arestas_str.substr(open_paren_pos + 1, close_paren_pos - open_paren_pos - 1);
        if (count(aresta.begin(), aresta.end(), ',') == 2) {
            return true;
        }

        open_paren_pos = arestas_str.find('(', close_paren_pos);
    }

    return false;
}
int main() {
    unsigned int numVertices = 25000; // Supondo um número máximo de vértices
    int opcao;
    string item1, item2;
    int nulo = 0;
    Grafo grafo(numVertices, nulo);

    
    char resposta;
    cout << "O grafo é direcionado? (y/n): ";
    cin >> resposta;
    if (resposta == 'y' || resposta == 'Y') {
        grafo.direcionado = true;
    }
    
    grafo.lergrafo();
    grafo.reescreverArquivo();
    
    
    if (grafo.direcionado){
    do {
        limpaTela();
        exibirMenuDirecionado();
        cin >> opcao;

        switch (opcao) {
            case 1: {
                animacaoTransicao();
                string vertice; 
                cout << "Digite o nome do vértice a ser inserido: ";
                cin >> vertice;
                grafo.verificaExistencia(vertice);
                grafo.reescreverArquivo();
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 2: {
                animacaoTransicao();
                string saida, entrada, peso;
                if (grafo.valorado == false){
                  cout << "Digite o nome do vértice de saída: ";
                  cin >> saida;
                  cout << "Digite o nome do vértice de entrada: ";
                  cin >> entrada;
                  peso = 1;
                  grafo.verificaExistenciaAresta(saida,entrada,peso);
                  grafo.reescreverArquivo();
                } else {
                
                cout << "Digite o nome do vértice de saída: ";
                cin >> saida;
                cout << "Digite o nome do vértice de entrada: ";
                cin >> entrada;
                cout << "Digite o peso dessa aresta: ";
                cin >> peso;
                grafo.verificaExistenciaAresta(saida,entrada, peso);
                grafo.reescreverArquivo();
                }
                
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
                grafo.obtergrau(vertice);             
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
            case 11: {
                animacaoTransicao();
                cout << "Digite o vertice de origem da aresta que vai ser apagada: ";
                cin >> item1;
                cout << "Digite o vertice de destino da aresta que vai ser apagada: ";
                cin >> item2;
                grafo.removeraresta(item1, item2);               
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 12: {
                animacaoTransicao();
                int b = grafo.ehConexo();
                if (b == 1){
                  cout << "O grafo é conexo" << endl;
                } else {
                  cout << "O grafo não é conexo" << endl;
                }
                cout << endl;
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 13: {
                animacaoTransicao();
                grafo.possuiciclos();
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 14: {
                animacaoTransicao();
                bool teste = grafo.ehfortementeconexo();
                if (teste){
                  cout << "Grafo é fortemente conexo" << endl;
                } else {
                  cout << "Grafo não é fortemente conexo" << endl;
                }
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 15: {
                animacaoTransicao();
                grafo.eheuleriano();
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 16: {
                animacaoTransicao();
                grafo.prim();
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 17: {
                animacaoTransicao();
                grafo.buscaemlarguraArvore();
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 18: {
                animacaoTransicao();
                grafo.buscaemprofundidadeArvore();
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 19:
                cout << "Obrigado por usar o nosso sitema =)" << endl;
                break;
            default:
                cout << "Opção inválida!" << endl;
        }
        usleep(500000); // Aguarda 5.5 segundos para dar tempo de ler antes de limpar a tela
    } while (opcao != 19);
    
    grafo.reescreverArquivo();
    } else {
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
                grafo.verificaExistencia(vertice);
                grafo.reescreverArquivo();
                
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 2: {
                animacaoTransicao();
                string saida, entrada, peso;
                if (grafo.valorado == false){
                  cout << "Digite o nome do vértice de saída: ";
                  cin >> saida;
                  cout << "Digite o nome do vértice de entrada: ";
                  cin >> entrada;
                  peso = 1;
                  grafo.verificaExistenciaAresta(saida,entrada,peso);
                  grafo.reescreverArquivo();
                } else {
                
                cout << "Digite o nome do vértice de saída: ";
                cin >> saida;
                cout << "Digite o nome do vértice de entrada: ";
                cin >> entrada;
                cout << "Digite o peso dessa aresta: ";
                cin >> peso;
                grafo.verificaExistenciaAresta(saida,entrada, peso);
                grafo.reescreverArquivo();
                }
                
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
                grafo.obtergrau(vertice);             
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
            case 11: {
                animacaoTransicao();
                cout << "Digite o vertice de origem da aresta que vai ser apagada: ";
                cin >> item1;
                cout << "Digite o vertice de destino da aresta que vai ser apagada: ";
                cin >> item2;
                grafo.removeraresta(item1, item2);               
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 12: {
                animacaoTransicao();
                int b = grafo.ehConexo();
                if (b == 1){
                  cout << "O grafo é conexo" << endl;
                } else {
                  cout << "O grafo não é conexo" << endl;
                }
                cout << endl;
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 13: {
                animacaoTransicao();
                grafo.possuiciclos();
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 14: {
                animacaoTransicao();
                grafo.eheuleriano();
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 15: {
                animacaoTransicao();
                grafo.prim();
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 16: {
                animacaoTransicao();
                grafo.buscaemlarguraArvore();
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 17: {
                animacaoTransicao();
                grafo.buscaemprofundidadeArvore();
                cout << "Pressione qualquer tecla para voltar ao menu principal...";
                cin.ignore();
                cin.get();
                break;
            }
            case 18:
                cout << "Obrigado por usar o nosso sitema =)" << endl;
                break;
            default:
                cout << "Opção inválida!" << endl;
        }
        usleep(500000); // Aguarda 5.5 segundos para dar tempo de ler antes de limpar a tela
    } while (opcao != 18);
    
    grafo.reescreverArquivo();

    }

  

    return 0;
}