// Arquivo grafo.cpp
#include "grafo.h"

Grafo::Grafo(int max) {
    numvertices = 0;
    maxvertices = max;
    arestanula = 0;
    direcionado = false; // Por padrão, o grafo não é direcionado

    vertices = new string[maxvertices];

    matrizadjacencias = new int* [maxvertices];
    for (int i = 0; i < maxvertices; i++) {
        matrizadjacencias[i] = new int[maxvertices];
    }

    for (int i = 0; i < maxvertices; i++) {
        for (int j = 0; j < maxvertices; j++) {
            matrizadjacencias[i][j] = arestanula;
        }
    }
}

Grafo::~Grafo() {
    delete[] vertices;
    for (int i = 0; i < maxvertices; i++) {
        delete[] matrizadjacencias[i];
    }
    delete[] matrizadjacencias;
}

int Grafo::obterindice(string item) {
    for (int i = 0; i < numvertices; i++) {
        if (vertices[i] == item) {
            return i;
        }
    }
    return -1; // Retorna -1 se o vértice não for encontrado
}

void Grafo::inserevertice(string item) {
    vertices[numvertices] = item;
    numvertices++;
}

void Grafo::inserearesta(string Nosaida, string Noentrada) {
    int linha = obterindice(Nosaida);
    int coluna = obterindice(Noentrada);

    if (linha != -1 and coluna != -1) {
        if (direcionado) {
            matrizadjacencias[linha][coluna] = 1; // Se há aresta, define como 1
        } else {
            matrizadjacencias[linha][coluna] = 1; // Para grafos não direcionados
            matrizadjacencias[coluna][linha] = 1; // Para grafos não direcionados
        }
    }
}

int Grafo::obtergrau(string item) {
    int linha = obterindice(item);
    int grau = 0;
    if (linha != -1) {
        for (int i = 0; i < maxvertices; i++) {
            if (matrizadjacencias[linha][i] != arestanula) {
                grau++;
            }
        }
    }
    return grau;
}

void Grafo::imprimirmatriz() {
    cout << "Matriz de adjacências:\n";
    for (int i = 0; i < numvertices; i++) {
        for (int j = 0; j < numvertices; j++) {
            cout << matrizadjacencias[i][j] << " ";
        }
        cout << endl;
    }
}

void Grafo::imprimirvertices() {
    cout << "Lista de Vértices:\n";
    for (int i = 0; i < numvertices; i++) {
        cout << vertices[i];
        for (int j = 0; j < numvertices; j++){
            if (matrizadjacencias[i][j] != 0){
                cout << "->" << vertices[j];
            }
        }
        cout << endl;
    }
}

void Grafo::lergrafo() {
    string arquivo = "grafo.txt";
    ifstream file(arquivo);

    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    string line;
    getline(file, line); // Lê toda a linha de vértices e arestas

    size_t pos = line.find("V = {") + 5; // Encontra o início dos vértices
    size_t end_pos = line.find("};");
    string vertices_str = line.substr(pos, end_pos - pos);

    // Processa os vértices
    string vertex;
    for (size_t i = 0; i <= vertices_str.size(); i++) {
        if (isdigit(vertices_str[i])) {
            vertex += vertices_str[i];
        }
        else if (!vertex.empty()) {
            inserevertice(vertex);
            vertex.clear();
        }
    }

    pos = line.find("A = {") + 5; // Encontra o início das arestas
    end_pos = line.find("};");
    string arestas_str = line.substr(pos, end_pos - pos);

    // Processa as arestas
    string aresta;
    string nos[2];
    int peso = 1; // Assumindo peso padrão de 1 para todas as arestas
    for (size_t i = 0; i <= arestas_str.size(); i++) {
        if (arestas_str[i] == '(') {
            i++; // Ignora o '('
            while (arestas_str[i] != ')') {
                if (isdigit(arestas_str[i])) {
                    aresta += arestas_str[i];
                }
                else if (!aresta.empty()) {
                    nos[0] = aresta;
                    aresta.clear();
                }
                i++;
            }
            if (!nos[0].empty()) {
                i++; // Pula o ','
                while (isdigit(arestas_str[i])) {
                    aresta += arestas_str[i];
                    i++;
                }
                nos[1] = aresta;
                inserearesta(nos[0], nos[1]);
                aresta.clear();
                nos[0].clear();
                nos[1].clear();
            }
        }
    }

    file.close();
}