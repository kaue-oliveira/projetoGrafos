#ifndef GRAFO_H
#define GRAFO_H

#include <string>
#include <iostream>
#include "lista.h"
#include <vector>
#include <string>
const std::string Nomearquivo = "grafo.txt";
using namespace std;
class Grafo {
public:
    int numvertices;
    int maxvertices;
    int arestanula;
    bool valorado;
    bool direcionado;
    std::string* vertices;
    bool* marcador;
    Lista* listaAdjacencia;

    int obterindice(std::string item);

    Grafo(int max, int nulo);
    ~Grafo();

    void inserevertice(std::string item);
    void inserearesta(std::string Nosaida, std::string Noentrada);
    void inserearestaValorado(std::string Nosaida, std::string Noentrada, std::string peso);
    void obtergrau(std::string item);
    int obtergrauaux(std::string item);
    void imprimirmatriz();
    void imprimirvertices();
    void removervertice(std::string verticeEditar);
    void removeraresta(std::string origem, std::string destino);
    bool ehConexo();
    void possuiCiclos();
    void eheuleriano();
    bool ehfortementeconexo();
    void lergrafo();
    void reescreverArquivo();
    void limpamarcador();
    void buscaemlarguraArvore();
    void buscaemprofundidadeArvore();
    void buscaemlargura(std::string origem, std::string destino);
    void buscaemprofundidade(std::string origem, std::string destino);
    void prim();
    int qtdvertice();
    int qtdarestas();
    void verificaExistencia(std::string vertice);
    void verificaExistenciaAresta(std::string saida, std::string entrada, std::string peso);
    bool verifica(const std::string& line);
    void tarjanArticulacao(int u, std::vector<int>& disc, std::vector<int>& low, std::vector<int>& pai, std::vector<bool>& articulacao, int& tempo);
    void encontrarVerticesArticulacao();
    void listarCaminhoHamiltoniano();
    bool hamiltonianUtil(int pos, std::vector<int>& caminho, std::vector<bool>& visitado);
    // novos testados ok
    void exibirCaminhoEuleriano();
    void exibirCaminhoMinimoDijkstra(string origem, string destino);
    void exibirOrdemTopologica();
    void exibirComponentesConexas();
    void exibirArestasPonte();

    // novos, testar
    int fluxoMaximo(std::string origem, std::string destino);
    Grafo fechamentoTransitivo();
    void tarjanPontesUtil(int u, std::vector<int>& disc, std::vector<int>& low, std::vector<int>& pai, std::vector<bool>& pontes, int& tempo, std::vector<std::pair<int, int>>& listaPontes);
    std::vector<std::vector<std::string>> componentesConexas();
    std::vector<std::string> caminhoEuleriano();
    std::vector<std::pair<std::string, std::string>> encontrarArestasPonte();
    void ehBipartido();
    bool ehHamiltoniano();
    bool ehCiclico();
    void ehPlanar();
    bool ehCiclicoUtil(int v, std::vector<bool>& visitado, std::vector<bool>& pilha);
    // implementar
    // void listarArestas();

};

#endif // GRAFO_H
