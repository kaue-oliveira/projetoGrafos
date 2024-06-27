#include <iostream>
#include <fstream>
#include <unistd.h> // Para o usleep
#include <SFML/Graphics.hpp>
#include "grafo.h"
#include "filadinamica.h"
#include "pilhadinamica.h"

using namespace std;

const unsigned int numVertices = 10000000; // Supondo um número máximo de vértices
int opcao;
bool direcionado = false, valorado = false;
Grafo grafo(numVertices, 0);

void exibirMenuPrincipal();
void exibirMenuVerificar();
void exibirMenuListar();
void exibirMenuGerar();

void limpaTela() 
{
  cout << "\033[2J\033[1;1H";
}

void animacaoTransicao() 
{
  limpaTela();
  cout << "\033[1m\033[32m"
        << "====================================================" << endl
        << "|                   Carregando...                   |" << endl
        << "====================================================" << endl
        << "\033[0m" << flush;
  usleep(1000000); // 1 segundo
}

void exibirMenu(const string& titulo, const vector<string>& opcoes) 
{
  limpaTela();
  cout << "\033[1m\033[33m"
        << "====================================================" << endl
        << "|            " << titulo << "            |" << endl
        << "====================================================" << endl
        << "\033[0m";

  cout << "\033[1m\033[34m";
  for (size_t i = 0; i < opcoes.size(); ++i) {
      cout << "| " << i + 1 << ". " << opcoes[i] << endl;
  }
  cout << "| 0. Sair                                          |" << endl;
  cout << "\033[0m\033[1m\033[33m"
        << "====================================================" << endl
        << "\033[0m";
  cout << "Escolha uma opção: ";
}


void processarOpcaoVerificar(int opcao) 
{
  switch (opcao) {
      case 1:
          animacaoTransicao();
          cout << "Quantidade de vértices: " << grafo.qtdvertice() << endl;
          break;
      case 2:
          animacaoTransicao();
          cout << "Quantidade de arestas: " << grafo.qtdarestas() << endl;
          break;
      case 3:
          animacaoTransicao();
          cout << (grafo.ehConexo() ? "O grafo é conexo" : "O grafo não é conexo") << endl;
          break;
      case 4:
          animacaoTransicao();
          grafo.ehBipartido();
          break;
      case 5:
          animacaoTransicao();
          grafo.eheuleriano();
          break;
      case 6:
          animacaoTransicao();
          grafo.ehHamiltoniano();
          break;
      case 7:
          animacaoTransicao();
          grafo.ehCiclico();
          break;
      case 8:
          animacaoTransicao();
          grafo.ehPlanar();
          break;
      case 0:
          cout << "Voltando para o menu principal..." << endl;
          exibirMenuPrincipal();
          return;
      default:
          cout << "Opção inválida!" << endl;
  }
  cout << "Pressione qualquer tecla para voltar ao menu...";
  cin.ignore();
  cin.get();
  exibirMenuVerificar();
}

void exibirMenuVerificar() 
{
  vector<string> opcoes = {
      "Quantidade de vértices", "Quantidade de arestas", "Conexo", "Bipartido",
      "Euleriano", "Hamiltoniano", "Cíclico", "Planar"
  };
  exibirMenu("M E N U   -   V E R I F I C A R", opcoes);
  cin >> opcao;
  processarOpcaoVerificar(opcao);
}

void processarOpcaoListar(int opcao) {
  switch (opcao) {
      case 1:
          animacaoTransicao();
          grafo.imprimirvertices();
          break;
      case 2:
          animacaoTransicao();
          grafo.listarArestas();
          break;
      case 3:
          animacaoTransicao();
          grafo.exibirComponentesConexas();
          break;
      case 4:
          animacaoTransicao();
          grafo.exibirCaminhoEuleriano();
          break;
      case 5:
          animacaoTransicao();
          grafo.listarCaminhoHamiltoniano();
          break;
      case 6:
          animacaoTransicao();
          grafo.encontrarVerticesArticulacao();
          break;
      case 7:
          animacaoTransicao();
          grafo.exibirArestasPonte();
          break;
      case 0:
          cout << "Voltando para o menu principal..." << endl;
          exibirMenuPrincipal();
          return;
      default:
          cout << "Opção inválida!" << endl;
  }
  cout << "Pressione qualquer tecla para voltar ao menu...";
  cin.ignore();
  cin.get();
  exibirMenuListar();
}

void exibirMenuListar() 
{
  vector<string> opcoes = {
      "Vértices", "Arestas", "Componentes conexas", "Um Caminho euleriano",
      "Vértices de articulação", "Arestas ponte"
  };
  exibirMenu("M E N U   -   L I S T A R", opcoes);
  cin >> opcao;
  processarOpcaoListar(opcao);
}

void processarOpcaoGerar(int opcao) {
  string origem, destino;
  switch (opcao) {
      case 1:
          animacaoTransicao();
          grafo.imprimirvertices();
          break;
      case 2:
          animacaoTransicao();
          grafo.buscaemprofundidadeArvore();
          break;
      case 3:
          animacaoTransicao();
          grafo.buscaemlarguraArvore();
          break;
      case 4:
          animacaoTransicao();
          grafo.prim();
          break;
      case 5:
          if (direcionado) {
              animacaoTransicao();
              grafo.exibirOrdemTopologica();
          } else {
              cout << "Opção inválida!" << endl;
          }
          break;
      case 6:
          if (valorado) {
              animacaoTransicao();
              cout << "Insira o vertice de origem: ";
              cin >> origem;
              cout << endl;
              cout << "Insira o vertice de destino: ";
              cin >> destino;
              grafo.exibirCaminhoMinimoDijkstra(origem, destino);
          } else {
              cout << "Opção inválida!" << endl;
          }
          break;
      case 7:
          if (valorado) {
              animacaoTransicao();
              cout << "Insira o vertice de origem: ";
              cin >> origem;
              cout << endl;
              cout << "Insira o vertice de destino: ";
              cin >> destino;
              int fluxoMax = grafo.fluxoMaximo(origem, destino);
              cout << "O fluxo maximo é: " << fluxoMax << endl;
          } else {
              cout << "Opção inválida!" << endl;
          }
          break;
      case 8:
          if (valorado) {
              animacaoTransicao();
              grafo.gerarFechamentoTransitivo();
          } else {
              cout << "Opção inválida!" << endl;
          }
          break;
      case 0:
          cout << "Voltando para o menu principal..." << endl;
          exibirMenuPrincipal();
          return;
      default:
          cout << "Opção inválida!" << endl;
  }
  cout << "Pressione qualquer tecla para voltar ao menu...";
  cin.ignore();
  cin.get();
  exibirMenuGerar();
}

void exibirMenuGerar() 
{
  vector<string> opcoes = {
      "Lista de adjacência", "Árvore de profundidade", "Árvore de largura", "Árvore geradora mínima"
  };
  if (direcionado) {
      opcoes.push_back("Ordem topológica");
  }
  if (valorado) {
      opcoes.push_back("Caminho mínimo entre vértices");
      opcoes.push_back("Fluxo máximo");
      opcoes.push_back("Fechamento transitivo");
  }
  exibirMenu("M E N U   -   G E R A R", opcoes);
  cin >> opcao;
  processarOpcaoGerar(opcao);
}

void exibirMenuPrincipal() 
{
  vector<string> opcoes = {
      "Verificar", "Listar", "Gerar"
  };
  exibirMenu("M E N U   -   P R I N C I P A L", opcoes);
  do {
      cin >> opcao;
      switch (opcao) {
          case 1:
              animacaoTransicao();
              exibirMenuVerificar();
              break;
          case 2:
              animacaoTransicao();
              exibirMenuListar();
              break;
          case 3:
              animacaoTransicao();
              exibirMenuGerar();
              break;
          case 0:
              cout << "Obrigado por usar o nosso sistema =)" << endl;
              return;
          default:
              cout << "Opção inválida!" << endl;
      }
      usleep(500000); // Aguarda 0.5 segundos para dar tempo de ler antes de limpar a tela
  } while (opcao != 0);
}

int main() {
    string resposta;
    cout << "O grafo é direcionado? (y/n): ";
    cin >> resposta;

    if (resposta == "y" || resposta == "Y") {
        grafo.defineDirecionado(resposta);
    }

    grafo.lergrafo();
    grafo.reescreverArquivo();

    if (grafo.ehdirecionado()) {
        direcionado = true;
    }
    if (grafo.ehvalorado()) {
        valorado = true;
    }

    exibirMenuPrincipal();
    grafo.reescreverArquivo();

    return 0;
}
