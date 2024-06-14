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
private:
    int numvertices;
    int maxvertices;
    int arestanula;
    bool valorado;
    bool direcionado;
    std::string* vertices;
    bool* marcador;
    Lista* listaAdjacencia;
public:
    // Essencial ou formação da lista
    Grafo(int max, int nulo);
    ~Grafo();
    void defineDirecionado(std::string resposta);
    void inserearesta(std::string Nosaida, std::string Noentrada);
    void inserearestaValorado(std::string Nosaida, std::string Noentrada, std::string peso);
    void inserevertice(std::string item);
    void removervertice(std::string verticeEditar);
    void removeraresta(std::string origem, std::string destino);
      
    // Manipulação de arquivo
    void lergrafo();
    void reescreverArquivo();
    bool verifica(const std::string& line);    
    
    // Uteis na manipulação do grafo
    int obterindice(std::string item);
    void obtergrau(std::string item);
    int obtergrauaux(std::string item);
    bool ehConexo();
    bool ehdirecionado();
    bool ehvalorado();
    int qtdvertice();
    int qtdarestas();
    void verificaExistencia(std::string vertice);
    void verificaExistenciaAresta(std::string saida, std::string entrada, std::string peso);
    int obtergrauEntrada(std::string item);
    int obtergrauSaida(std::string item);   
    void limpamarcador();
    
    //exibir 
    void imprimirvertices();
    void possuiCiclos();
    void eheuleriano();
    bool ehfortementeconexo();
    bool ehCiclicoUtil(int v, std::vector<bool>& visitado, std::vector<bool>& pilha);
    bool eheulerianoAux();
    void ehCiclico();
    void ehPlanar();    
    void buscaemlarguraArvore();
    void buscaemprofundidadeArvore();
    void buscaemlargura(std::string origem, std::string destino);
    void buscaemprofundidade(std::string origem, std::string destino);
    void prim();    
    void tarjanArticulacao(int u, std::vector<int>& disc, std::vector<int>& low, std::vector<int>& pai, std::vector<bool>& articulacao, int& tempo);
    void encontrarVerticesArticulacao();
    void listarCaminhoHamiltoniano();
    bool hamiltonianUtil(int pos, std::vector<int>& caminho, std::vector<bool>& visitado);    
    bool ehHamiltoniano();
    void exibirCaminhoEuleriano();
    void exibirCaminhoMinimoDijkstra(string origem, string destino);
    void exibirOrdemTopologica();
    void exibirComponentesConexas();
    void exibirArestasPonte();
    void tarjanPontesUtil(int u, std::vector<int>& disc, std::vector<int>& low, std::vector<int>& pai, 
    std::vector<bool>& pontes, int& tempo, std::vector<std::pair<int, int>>& listaPontes);    
    void ehBipartido();
    
    
    // implementar ou arrumar ou apagar
    // void listarArestas();
    int fluxoMaximo(std::string origem, std::string destino);
    Grafo fechamentoTransitivo();
    void imprimirmatriz();

};

#endif // GRAFO_H