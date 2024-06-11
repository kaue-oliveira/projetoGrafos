#include "grafo.h"
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <climits>
#include <vector>
#include <chrono>
#include <algorithm>
#include "pilhadinamica.h"
#include "filadinamica.h"

using namespace std::chrono;
using namespace std;

Grafo::Grafo(int max, int nulo) {
    numvertices = 0;
    maxvertices = max;
    arestanula = nulo;
    valorado = false;
    direcionado = false;
    vertices = new string[maxvertices];
    marcador = new bool[maxvertices];
    listaAdjacencia = new Lista[maxvertices];
}

Grafo::~Grafo() {
    delete[] vertices;
    delete[] marcador;
    delete[] listaAdjacencia;
}

int Grafo::obterindice(string item) {
    for (int i = 0; i < numvertices; i++) {
        if (vertices[i] == item) {
            return i;
        }
    }
    return -1;
}

void Grafo::inserevertice(string item) {
    if (obterindice(item) != -1) {
        cerr << "Ocorreu um erro pois esse vertice ja existe." << endl;
        return;
    }
    vertices[numvertices] = item;
    numvertices++;
}

void Grafo::inserearesta(string Nosaida, string Noentrada) {
    int linha = obterindice(Nosaida);
    int coluna = obterindice(Noentrada);

    if (linha != -1 && coluna != -1) {
        listaAdjacencia[linha].inserir(coluna, 1);
        if (!direcionado) {
            listaAdjacencia[coluna].inserir(linha, 1);
        }
    }
}

void Grafo::inserearestaValorado(string Nosaida, string Noentrada, string peso) {
    int linha = obterindice(Nosaida);
    int coluna = obterindice(Noentrada);
    int pesoNovo = stoi(peso);

    if (linha != -1 && coluna != -1) {
        listaAdjacencia[linha].inserir(coluna, pesoNovo);
        if (!direcionado) {
            listaAdjacencia[coluna].inserir(linha, pesoNovo);
        }
    }
}

void Grafo::obtergrau(string item) {
    int linha = obterindice(item);
    if (linha == -1) {
        cout << "O vertice " << item << " não existe no grafo." << endl;
        return;
    }

    if (!direcionado) {
        int grau = listaAdjacencia[linha].tamanho();
        cout << "O vertice " << item << " tem grau " << grau << endl;
    } else {
        int saida = listaAdjacencia[linha].tamanho();
        int entrada = 0;
        for (int i = 0; i < numvertices; i++) {
            entrada += listaAdjacencia[i].contar(linha);
        }
        cout << "O vertice " << item << " tem grau de saída " << saida << " e grau de entrada " << entrada << endl;
    }
}

int Grafo::obtergrauaux(string item) {
    int linha = obterindice(item);
    if (linha == -1) {
        return 0;
    }
    return listaAdjacencia[linha].tamanho();
}

void Grafo::imprimirmatriz() {
    cout << "Função removida, pois a matriz de adjacências não é mais usada." << endl;
}

void Grafo::imprimirvertices() {
    cout << "Lista de Vértices:\n";
    for (int i = 0; i < numvertices; i++) {
        cout << vertices[i];
        listaAdjacencia[i].imprimir(vertices);
        cout << endl;
    }
}

void Grafo::removervertice(string verticeEditar) {
    int indiceVertice = obterindice(verticeEditar);
    if (indiceVertice == -1) {
        cerr << "O vértice não existe =(" << endl;
        return;
    }

    listaAdjacencia[indiceVertice].limpar();
    for (int i = 0; i < numvertices; i++) {
        listaAdjacencia[i].remover(indiceVertice);
    }

    for (int i = indiceVertice; i < numvertices - 1; i++) {
        vertices[i] = vertices[i + 1];
        listaAdjacencia[i] = listaAdjacencia[i + 1];
    }
    numvertices--;
    reescreverArquivo();
}

void Grafo::removeraresta(string origem, string destino) {
    int verticeorigem = obterindice(origem);
    int verticedestino = obterindice(destino);
    if (verticeorigem == -1 || verticedestino == -1) {
        cerr << "Um dos vértices não existe." << endl;
        return;
    }

    listaAdjacencia[verticeorigem].remover(verticedestino);

    if (!direcionado) {
        listaAdjacencia[verticedestino].remover(verticeorigem);
    }
    reescreverArquivo();
}

bool Grafo::ehConexo() {
    if (numvertices == 0) {
        return true;
    }

    vector<bool> visitados(numvertices, false);
    queue<int> fila;
    fila.push(0);
    visitados[0] = true;

    while (!fila.empty()) {
        int verticeAtual = fila.front();
        fila.pop();

        Lista::No* adj = listaAdjacencia[verticeAtual].inicio();
        while (adj != nullptr) {
            int adjVertice = adj->vertice;
            if (!visitados[adjVertice]) {
                fila.push(adjVertice);
                visitados[adjVertice] = true;
            }
            adj = adj->proximo;
        }
    }

    for (bool visitado : visitados) {
        if (!visitado) {
            return false;
        }
    }
    return true;
}

void Grafo::possuiCiclos() {
    int contador = 0;
    for (int i = 0; i < numvertices; i++) {
        Lista::No* adj = listaAdjacencia[i].inicio();
        while (adj != nullptr) {
            if (adj->vertice == i) {
                contador++;
            }
            adj = adj->proximo;
        }
    }
    cout << "O grafo possui " << contador << " ciclos" << endl;
}

void Grafo::eheuleriano() {
    int grauImpar = 0;
    for (int i = 0; i < numvertices; i++) {
        int grau = obtergrauaux(vertices[i]);
        if (grau % 2 != 0) {
            grauImpar++;
        }
    }
    if (grauImpar == 0) {
        cout << "Grafo é euleriano" << endl;
    } else if (grauImpar == 2) {
        cout << "Grafo é semieuleriano" << endl;
    } else {
        cout << "Grafo não é euleriano" << endl;
    }
}

bool Grafo::ehfortementeconexo() {
    if (numvertices == 0) {
        return true;
    }

    vector<bool> visitados(numvertices, false);

    auto dfs = [&](int inicio) {
        stack<int> pilha;
        pilha.push(inicio);
        visitados[inicio] = true;

        while (!pilha.empty()) {
            int u = pilha.top();
            pilha.pop();

            Lista::No* adj = listaAdjacencia[u].inicio();
            while (adj != nullptr) {
                int v = adj->vertice;
                if (!visitados[v]) {
                    pilha.push(v);
                    visitados[v] = true;
                }
                adj = adj->proximo;
            }
        }
    };

    dfs(0);
    for (bool visitado : visitados) {
        if (!visitado) {
            return false;
        }
    }

    fill(visitados.begin(), visitados.end(), false);
    dfs(1);
    for (bool visitado : visitados) {
        if (!visitado) {
            return false;
        }
    }

    return true;
}

void Grafo::lergrafo() {
    ifstream file(Nomearquivo);
    if (!file.is_open()) {
        cerr << "Não foi possível abrir o arquivo " << Nomearquivo << "." << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    getline(file, line);

    size_t vertices_pos = line.find("V = {");
    size_t arestas_pos = line.find("}; A = {");

    if (vertices_pos == string::npos || arestas_pos == string::npos) {
        cerr << "Erro de formatação." << endl;
        file.close();
        exit(EXIT_FAILURE);
    }

    bool isValorado = verifica(line);
    valorado = isValorado;

    size_t pos = vertices_pos + 5;
    size_t end_pos = line.find("}", pos);
    string vertices_str = line.substr(pos, end_pos - pos);

    stringstream ss(vertices_str);
    string vertex;
    while (getline(ss, vertex, ',')) {
        inserevertice(vertex);
    }

    pos = arestas_pos + 6;
    end_pos = line.find("};", pos);
    string arestas_content = line.substr(pos, end_pos - pos);

    stringstream arestas_ss(arestas_content);
    string aresta;
    while (getline(arestas_ss, aresta, ')')) {
        size_t open_pos = aresta.find('(');
        if (open_pos != string::npos) {
            string elementos = aresta.substr(open_pos + 1);
            stringstream elementos_ss(elementos);
            string no1, no2, valor;

            getline(elementos_ss, no1, ',');
            getline(elementos_ss, no2, ',');

            if (isValorado) {
                getline(elementos_ss, valor, ',');
                inserearestaValorado(no1, no2, valor);
            } else {
                inserearesta(no1, no2);
            }
        }
    }
    file.close();
}

void Grafo::reescreverArquivo() {
    ofstream arquivo(Nomearquivo);
    if (!arquivo.is_open()) {
        cerr << "Não foi possível abrir o arquivo " << Nomearquivo << "." << endl;
        return;
    }

    arquivo << "V = {";
    for (int i = 0; i < numvertices; i++) {
        arquivo << vertices[i];
        if (i != numvertices - 1) {
            arquivo << ",";
        }
    }
    arquivo << "}";

    bool isValorado = valorado;

    arquivo << "; A = {";
    bool temArestas = false;
    for (int i = 0; i < numvertices; i++) {
        Lista::No* adj = listaAdjacencia[i].inicio();
        while (adj != nullptr) {
            int j = adj->vertice;
            temArestas = true;
            arquivo << "(" << vertices[i] << "," << vertices[j];
            if (isValorado) {
                arquivo << "," << adj->peso;
            }
            arquivo << "),";
            adj = adj->proximo;
        }
    }

    if (temArestas) {
        arquivo.seekp(-1, ios_base::end);
    }
    arquivo << "};";
    arquivo.close();
}

void Grafo::limpamarcador() {
    fill(marcador, marcador + maxvertices, false);
}

void Grafo::buscaemlarguraArvore() {
    auto start_time = high_resolution_clock::now();
    vector<int> pais(numvertices, -1);
    vector<int> distancia(numvertices, INT_MAX);
    vector<bool> visitado(numvertices, false);

    auto executarBuscaEmLargura = [&](int inicio) {
        queue<int> fila;
        fila.push(inicio);
        visitado[inicio] = true;
        distancia[inicio] = 0;

        while (!fila.empty()) {
            int u = fila.front();
            fila.pop();

            Lista::No* adj = listaAdjacencia[u].inicio();
            while (adj != nullptr) {
                int v = adj->vertice;
                if (!visitado[v]) {
                    fila.push(v);
                    visitado[v] = true;
                    distancia[v] = distancia[u] + 1;
                    pais[v] = u;
                }
                adj = adj->proximo;
            }
        }
    };

    for (int i = 0; i < numvertices; i++) {
        if (!visitado[i]) {
            executarBuscaEmLargura(i);
        }
    }

    cout << "Árvore de busca em largura:\n";
    for (int i = 0; i < numvertices; ++i) {
    if (pais[i] != -1 || (pais[i] == -1 && listaAdjacencia[i].tamanho() > 0)) {
        cout << "(" << (pais[i] != -1 ? vertices[pais[i]] : vertices[i]) << ", " << vertices[i] << ") - Distância: " << distancia[i] << "\n";
    } else {
        cout << "(" << vertices[i] << ") - Isolado\n";
    }
}


    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end_time - start_time);
    cout << "Tempo total de execução: " << duration.count() << " segundos" << endl;
}

void Grafo::buscaemprofundidadeArvore() {
    auto start_time = high_resolution_clock::now();
    vector<int> pais(numvertices, -1);
    vector<int> distancia(numvertices, INT_MAX);
    vector<bool> visitado(numvertices, false);

    auto executarBuscaEmProfundidade = [&](int inicio) {
        stack<int> pilha;
        pilha.push(inicio);
        visitado[inicio] = true;
        distancia[inicio] = 0;

        while (!pilha.empty()) {
            int u = pilha.top();
            pilha.pop();

            Lista::No* adj = listaAdjacencia[u].inicio();
            while (adj != nullptr) {
                int v = adj->vertice;
                if (!visitado[v]) {
                    pilha.push(v);
                    visitado[v] = true;
                    distancia[v] = distancia[u] + 1;
                    pais[v] = u;
                }
                adj = adj->proximo;
            }
        }
    };

    for (int i = 0; i < numvertices; i++) {
        if (!visitado[i]) {
            executarBuscaEmProfundidade(i);
        }
    }

    cout << "Árvore de busca em profundidade:\n";
    for (int i = 0; i < numvertices; ++i) {
    if (pais[i] != -1 || (pais[i] == -1 && listaAdjacencia[i].tamanho() > 0)) {
        cout << "(" << (pais[i] != -1 ? vertices[pais[i]] : vertices[i]) << ", " << vertices[i] << ") - Distância: " << distancia[i] << "\n";
    } else {
        cout << "(" << vertices[i] << ") - Isolado\n";
    }
}

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end_time - start_time);
    cout << "Tempo total de execução: " << duration.count() << " segundos" << endl;
}

void Grafo::buscaemlargura(string origem, string destino) {
    auto start_time = high_resolution_clock::now();
    filadinamica filaVertices;
    bool encontrado = false;
    limpamarcador();
    filaVertices.inserir(origem);
    do {
        string verticeAtual = filaVertices.remover();
        if (verticeAtual == destino) {
            cout << "Visitando: " << verticeAtual << endl;
            cout << "Caminho encontrado!\n";
            encontrado = true;
        } else {
            int indice = obterindice(verticeAtual);
            cout << "Visitando: " << verticeAtual << endl;
            Lista::No* adj = listaAdjacencia[indice].inicio();
            while (adj != nullptr) {
                int adjIndice = adj->vertice;
                if (!marcador[adjIndice]) {
                    cout << "Enfileirando: " << vertices[adjIndice] << endl;
                    filaVertices.inserir(vertices[adjIndice]);
                    marcador[adjIndice] = true;
                }
                adj = adj->proximo;
            }
        }
    } while (!filaVertices.estavazio() && !encontrado);
    if (!encontrado) {
        cout << "Caminho não encontrado!\n";
    }
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end_time - start_time);
    cout << "Tempo total de execução: " << duration.count() << " segundos" << endl;
}

void Grafo::buscaemprofundidade(string origem, string destino) {
    auto start_time = high_resolution_clock::now();
    pilhadinamica pilhaVertices;
    bool encontrado = false;
    limpamarcador();
    pilhaVertices.inserir(origem);
    do {
        string verticeAtual = pilhaVertices.remover();
        if (verticeAtual == destino) {
            cout << "Visitando: " << verticeAtual << endl;
            cout << "Caminho encontrado!\n";
            encontrado = true;
        } else {
            int indice = obterindice(verticeAtual);
            cout << "Visitando: " << verticeAtual << endl;
            Lista::No* adj = listaAdjacencia[indice].inicio();
            while (adj != nullptr) {
                int adjIndice = adj->vertice;
                if (!marcador[adjIndice]) {
                    cout << "Empilhando: " << vertices[adjIndice] << endl;
                    pilhaVertices.inserir(vertices[adjIndice]);
                    marcador[adjIndice] = true;
                }
                adj = adj->proximo;
            }
        }
    } while (!pilhaVertices.estavazio() && !encontrado);
    if (!encontrado) {
        cout << "Caminho não encontrado!\n";
    }
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end_time - start_time);
    cout << "Tempo total de execução: " << duration.count() << " segundos" << endl;
}

void Grafo::prim() {
  if (ehConexo()){
    auto start_time = high_resolution_clock::now();
    vector<int> pais(numvertices, -1);
    vector<int> distancia(numvertices, INT_MAX);
    vector<bool> naArvore(numvertices, false);

    auto executarPrim = [&](int inicio) {
        distancia[inicio] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> filaPrioridade;
        filaPrioridade.push(make_pair(distancia[inicio], inicio));

        while (!filaPrioridade.empty()) {
            int u = filaPrioridade.top().second;
            filaPrioridade.pop();

            if (naArvore[u]) continue;
            naArvore[u] = true;

            Lista::No* adj = listaAdjacencia[u].inicio();
            while (adj != nullptr) {
                int v = adj->vertice;
                int peso = adj->peso;
                if (!naArvore[v] && peso < distancia[v]) {
                    pais[v] = u;
                    distancia[v] = peso;
                    filaPrioridade.push(make_pair(distancia[v], v));
                }
                adj = adj->proximo;
            }
        }
    };

    for (int i = 0; i < numvertices; i++) {
        if (!naArvore[i]) {
            executarPrim(i);
        }
    }

    cout << "Arestas da árvore mínima:\n";
    for (int i = 0; i < numvertices; ++i) {
        if (pais[i] != -1) {
            cout << "(" << vertices[pais[i]] << ", " << vertices[i] << ") - Peso: " << distancia[i] << "\n";
        }
    }

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end_time - start_time);
    cout << "Tempo total de execução: " << duration.count() << " segundos" << endl;
  } else {
    cerr << "Grafo não é conexo, logo não é possivel executar prim. " << endl;
  }
}

int Grafo::qtdvertice() {
    return numvertices;
}

int Grafo::qtdarestas() {
    int contadorArestas = 0;
    for (int i = 0; i < numvertices; i++) {
        contadorArestas += listaAdjacencia[i].tamanho();
    }
    return contadorArestas;
}

void Grafo::verificaExistencia(string vertice) {
    bool teste = true;
    for (int i = 0; i < numvertices; i++) {
        if (vertice == vertices[i]) {
            teste = false;
        }
    }

    if (teste) {
        inserevertice(vertice);
    } else {
        cerr << "Esse nome de vertice ja existe, tente novamente" << endl;
    }
}

void Grafo::verificaExistenciaAresta(string saida, string entrada, string peso) {
    if (!valorado) {
        bool testeSaida = false;
        bool testeEntrada = true;
        for (int i = 0; i < numvertices; i++) {
            if (vertices[i] == saida) {
                testeSaida = true;
            }
            if (vertices[i] == entrada) {
                testeEntrada = true;
            }
        }
        if (testeEntrada && testeSaida) {
            inserearesta(saida, entrada);
        } else {
            cerr << "Um dos vertices de entrada ou saida, não existe" << endl;
        }
    } else {
        bool testeSaida = false;
        bool testeEntrada = true;
        for (int i = 0; i < numvertices; i++) {
            if (vertices[i] == saida) {
                testeSaida = true;
            }
            if (vertices[i] == entrada) {
                testeEntrada = true;
            }
        }
        if (testeEntrada && testeSaida) {
            inserearestaValorado(saida, entrada, peso);
        } else {
            cerr << "Um dos vertices de entrada ou saida, não existe" << endl;
        }
    }
}

bool Grafo::verifica(const string& line) {
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
void Grafo::tarjanArticulacao(int u, vector<int>& disc, vector<int>& low, vector<int>& pai, vector<bool>& articulacao, int& tempo) {
    int filhos = 0;
    disc[u] = low[u] = ++tempo;

    Lista::No* adj = listaAdjacencia[u].inicio();
    while (adj != nullptr) {
        int v = adj->vertice;

        if (disc[v] == -1) {
            filhos++;
            pai[v] = u;
            tarjanArticulacao(v, disc, low, pai, articulacao, tempo);
            low[u] = min(low[u], low[v]);

            if (pai[u] == -1 && filhos > 1) {
                articulacao[u] = true;
            }
            if (pai[u] != -1 && low[v] >= disc[u]) {
                articulacao[u] = true;
            }
        } else if (v != pai[u]) {
            low[u] = min(low[u], disc[v]);
        }
        adj = adj->proximo;
    }
}

void Grafo::encontrarVerticesArticulacao() {
    vector<int> disc(numvertices, -1);
    vector<int> low(numvertices, -1);
    vector<int> pai(numvertices, -1);
    vector<bool> articulacao(numvertices, false);

    int tempo = 0;

    for (int i = 0; i < numvertices; i++) {
        if (disc[i] == -1) {
            tarjanArticulacao(i, disc, low, pai, articulacao, tempo);
        }
    }

    cout << "Vértices de articulação: ";
    for (int i = 0; i < numvertices; i++) {
        if (articulacao[i]) {
            cout << vertices[i] << " ";
        }
    }
    cout << endl;
}




void Grafo::listarCaminhoHamiltoniano() {
    vector<int> caminho(numvertices, -1);
    vector<bool> visitado(numvertices, false);

    caminho[0] = 0;
    visitado[0] = true;

    if (hamiltonianUtil(1, caminho, visitado)) {
        cout << "Caminho Hamiltoniano encontrado: ";
        for (int i = 0; i < numvertices; i++) {
            cout << vertices[caminho[i]] << " " << endl;
        }
        cout << vertices[caminho[0]] << endl;
    } else {
        cout << "Nenhum Caminho Hamiltoniano encontrado." << endl;
    }
}

void Grafo::exibirOrdemTopologica() {
    vector<int> grauEntrada(numvertices, 0);
    for (int i = 0; i < numvertices; i++) {
        Lista::No* adj = listaAdjacencia[i].inicio();
        while (adj != nullptr) {
            grauEntrada[adj->vertice]++;
            adj = adj->proximo;
        }
    }

    queue<int> fila;
    for (int i = 0; i < numvertices; i++) {
        if (grauEntrada[i] == 0) {
            fila.push(i);
        }
    }

    vector<int> ordem;
    while (!fila.empty()) {
        int u = fila.front();
        fila.pop();
        ordem.push_back(u);

        Lista::No* adj = listaAdjacencia[u].inicio();
        while (adj != nullptr) {
            grauEntrada[adj->vertice]--;
            if (grauEntrada[adj->vertice] == 0) {
                fila.push(adj->vertice);
            }
            adj = adj->proximo;
        }
    }

    if (ordem.size() != numvertices) {
        cerr << "O grafo possui ciclos e portanto não tem uma ordem topológica válida." << endl;
        return;
    }
    for (int i = 0; i < numvertices; i++){
      if (i != numvertices-1)
        cout << ordem[i] << " -> ";
      else 
        cout << ordem[i];

    }
    cout << endl;
}


void Grafo::exibirCaminhoMinimoDijkstra(string origem, string destino) {
    int origemIdx = obterindice(origem);
    int destinoIdx = obterindice(destino);

    if (origemIdx == -1 || destinoIdx == -1) {
        cerr << "Um dos vértices não existe." << endl;
        return;
    }

    vector<int> distancia(numvertices, INT_MAX);
    vector<int> pais(numvertices, -1);
    vector<bool> visitado(numvertices, false);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> filaPrioridade;
    distancia[origemIdx] = 0;
    filaPrioridade.push(make_pair(0, origemIdx));

    while (!filaPrioridade.empty()) {
        int u = filaPrioridade.top().second;
        filaPrioridade.pop();

        if (visitado[u]) continue;
        visitado[u] = true;

        Lista::No* adj = listaAdjacencia[u].inicio();
        while (adj != nullptr) {
            int v = adj->vertice;
            int peso = adj->peso;

            if (!visitado[v] && distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                pais[v] = u;
                filaPrioridade.push(make_pair(distancia[v], v));
            }
            adj = adj->proximo;
        }
    }

    vector<int> caminho;
    for (int v = destinoIdx; v != -1; v = pais[v]) {
        caminho.push_back(v);
    }
    reverse(caminho.begin(), caminho.end());

    if (caminho.size() == 1 && caminho[0] != origemIdx) {
        cerr << "Caminho não encontrado." << endl;
        return;
    }

    cout << "Caminho mínimo de " << origem << " para " << destino << ": ";
    for (int i = 0; i < caminho.size(); i++) {
        cout << caminho[i];
        if (i < caminho.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
}

int Grafo::fluxoMaximo(string origem, string destino) {
    int origemIdx = obterindice(origem);
    int destinoIdx = obterindice(destino);

    if (origemIdx == -1 || destinoIdx == -1) {
        cerr << "Um dos vértices não existe." << endl;
        return 0;
    }

    vector<vector<int>> capacidade(numvertices, vector<int>(numvertices, 0));
    for (int i = 0; i < numvertices; i++) {
        Lista::No* adj = listaAdjacencia[i].inicio();
        while (adj != nullptr) {
            capacidade[i][adj->vertice] = adj->peso;
            adj = adj->proximo;
        }
    }

    int fluxoMaximo = 0;
    vector<int> pai(numvertices);
    auto bfs = [&](int s, int t) {
        fill(pai.begin(), pai.end(), -1);
        queue<pair<int, int>> q;
        q.push({s, INT_MAX});
        while (!q.empty()) {
            int u = q.front().first;
            int fluxo = q.front().second;
            q.pop();

            for (int v = 0; v < numvertices; v++) {
                if (pai[v] == -1 && capacidade[u][v]) {
                    pai[v] = u;
                    int novoFluxo = min(fluxo, capacidade[u][v]);
                    if (v == t) {
                        return novoFluxo;
                    }
                    q.push({v, novoFluxo});
                }
            }
        }
        return 0;
    };

    int fluxo;
    while ((fluxo = bfs(origemIdx, destinoIdx))) {
        fluxoMaximo += fluxo;
        int u = destinoIdx;
        while (u != origemIdx) {
            int v = pai[u];
            capacidade[v][u] -= fluxo;
            capacidade[u][v] += fluxo;
            u = v;
        }
    }

    return fluxoMaximo;
}

Grafo Grafo::fechamentoTransitivo() {
    Grafo fechamento(numvertices, arestanula);
    for (int i = 0; i < numvertices; i++) {
        fechamento.inserevertice(vertices[i]);
    }

    for (int i = 0; i < numvertices; i++) {
        vector<bool> alcancavel(numvertices, false);
        queue<int> fila;
        fila.push(i);
        alcancavel[i] = true;

        while (!fila.empty()) {
            int u = fila.front();
            fila.pop();
            Lista::No* adj = listaAdjacencia[u].inicio();
            while (adj != nullptr) {
                int v = adj->vertice;
                if (!alcancavel[v]) {
                    alcancavel[v] = true;
                    fila.push(v);
                }
                adj = adj->proximo;
            }
        }

        for (int j = 0; j < numvertices; j++) {
            if (alcancavel[j]) {
                fechamento.inserearesta(vertices[i], vertices[j]);
            }
        }
    }

    return fechamento;
}



// Função para encontrar componentes conexas usando busca em profundidade
void dfsComponentes(int v, vector<bool>& visitado, vector<string>& componente, Lista* listaAdjacencia, string* vertices) {
    visitado[v] = true;
    componente.push_back(vertices[v]);
    Lista::No* adj = listaAdjacencia[v].inicio();
    while (adj != nullptr) {
        if (!visitado[adj->vertice]) {
            dfsComponentes(adj->vertice, visitado, componente, listaAdjacencia, vertices);
        }
        adj = adj->proximo;
    }
}

void Grafo::exibirComponentesConexas() {
    vector<bool> visitado(numvertices, false);
    vector<vector<string>> componentes;

    for (int v = 0; v < numvertices; v++) {
        if (!visitado[v]) {
            vector<string> componente;
            dfsComponentes(v, visitado, componente, listaAdjacencia, vertices);
            componentes.push_back(componente);
        }
    }

    cout << "Componentes Conexas:" << endl;
    for (int i = 0; i < componentes.size(); i++) {
        cout << "Componente " << i + 1 << ": ";
        for (int j = 0; j < componentes[i].size(); j++) {
            cout << componentes[i][j];
            if (j < componentes[i].size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    }
}

void Grafo::exibirCaminhoEuleriano() {
    vector<int> grau(numvertices, 0);
    int verticesImpar = 0;
    int inicio = 0;

    for (int i = 0; i < numvertices; i++) {
        grau[i] = listaAdjacencia[i].tamanho();
        if (grau[i] % 2 != 0) {
            verticesImpar++;
            inicio = i;
        }
    }

    if (verticesImpar != 0 && verticesImpar != 2) {
        cerr << "O grafo não possui um caminho Euleriano." << endl;
        return;
    }

    vector<string> caminho;
    stack<int> pilha;
    vector<vector<int>> adj(numvertices);
    for (int i = 0; i < numvertices; i++) {
        Lista::No* adjNode = listaAdjacencia[i].inicio();
        while (adjNode != nullptr) {
            adj[i].push_back(adjNode->vertice);
            adjNode = adjNode->proximo;
        }
    }

    pilha.push(inicio);
    while (!pilha.empty()) {
        int v = pilha.top();
        if (adj[v].empty()) {
            caminho.push_back(vertices[v]);
            pilha.pop();
        } else {
            int u = adj[v].back();
            adj[v].pop_back();
            pilha.push(u);
        }
    }

    cout << "Caminho Euleriano:" << endl;
    for (int i = 0; i < caminho.size(); i++) {
        cout << caminho[i];
        if (i < caminho.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
}

// Função para encontrar arestas ponte usando o algoritmo de Tarjan
void Grafo::tarjanPontesUtil(int u, vector<int>& disc, vector<int>& low, vector<int>& pai, vector<bool>& pontes, int& tempo, vector<pair<int, int>>& listaPontes) {
    static const int NIL = -1;
    disc[u] = low[u] = ++tempo;

    Lista::No* adj = listaAdjacencia[u].inicio();
    while (adj != nullptr) {
        int v = adj->vertice;
        if (disc[v] == NIL) {
            pai[v] = u;
            tarjanPontesUtil(v, disc, low, pai, pontes, tempo, listaPontes);
            low[u] = min(low[u], low[v]);

            if (low[v] > disc[u]) {
                listaPontes.push_back({u, v});
            }
        } else if (v != pai[u]) {
            low[u] = min(low[u], disc[v]);
        }
        adj = adj->proximo;
    }
}

void Grafo::exibirArestasPonte() {
    vector<int> disc(numvertices, -1);
    vector<int> low(numvertices, -1);
    vector<int> pai(numvertices, -1);
    vector<bool> pontes(numvertices, false);
    vector<pair<int, int>> listaPontes;
    int tempo = 0;

    for (int i = 0; i < numvertices; i++) {
        if (disc[i] == -1) {
            tarjanPontesUtil(i, disc, low, pai, pontes, tempo, listaPontes);
        }
    }

    cout << "Arestas Ponte:" << endl;
    for (auto& ponte : listaPontes) {
        cout << vertices[ponte.first] << " - " << vertices[ponte.second] << endl;
    }
}

// Função tarjanPontesUtil aqui...


// Função para verificar se o grafo é bipartido usando BFS
void Grafo::ehBipartido() {
  bool testeBipartido = true;
    vector<int> cor(numvertices, -1);

    for (int i = 0; i < numvertices; i++) {
        if (cor[i] == -1) {
            queue<int> fila;
            fila.push(i);
            cor[i] = 0;

            while (!fila.empty()) {
                int u = fila.front();
                fila.pop();

                Lista::No* adj = listaAdjacencia[u].inicio();
                while (adj != nullptr) {
                    int v = adj->vertice;
                    if (cor[v] == -1) {
                        cor[v] = 1 - cor[u];
                        fila.push(v);
                    } else if (cor[v] == cor[u]) {
                        testeBipartido = false;
                    }
                    adj = adj->proximo;
                }
            }
        }
    }
    if (testeBipartido){
      cout << "Grafo é Bipartido. " << endl;
    } else {
      cout << "Este grafo não é bipartido. " << endl;
    }
}

// Função para verificar se o grafo é Hamiltoniano usando backtracking
bool Grafo::ehHamiltoniano() {
    vector<int> caminho(numvertices, -1);
    vector<bool> visitado(numvertices, false);

    caminho[0] = 0;
    visitado[0] = true;

    return hamiltonianUtil(1, caminho, visitado);
}

// Função auxiliar para verificar se o grafo é Hamiltoniano
bool Grafo::hamiltonianUtil(int pos, std::vector<int>& caminho, std::vector<bool>& visitado) {
    if (pos == numvertices) {
        return listaAdjacencia[caminho[pos - 1]].contar(caminho[0]);
    }

    for (int v = 0; v < numvertices; v++) {
        if (!visitado[v] && listaAdjacencia[caminho[pos - 1]].contar(v)) {
            caminho[pos] = v;
            visitado[v] = true;
            if (hamiltonianUtil(pos + 1, caminho, visitado)) {
                return true;
            }
            visitado[v] = false;
        }
    }
    return false;
}

// Função para verificar se o grafo é cíclico usando DFS
bool Grafo::ehCiclico() {
    vector<bool> visitado(numvertices, false);
    vector<bool> pilha(numvertices, false);

    for (int i = 0; i < numvertices; i++) {
        if (!visitado[i]) {
            if (ehCiclicoUtil(i, visitado, pilha)) {
                return true;
            }
        }
    }
    return false;
}

// Função auxiliar para verificar se o grafo é cíclico
bool Grafo::ehCiclicoUtil(int v, vector<bool>& visitado, vector<bool>& pilha) {
    if (!visitado[v]) {
        visitado[v] = true;
        pilha[v] = true;

        Lista::No* adj = listaAdjacencia[v].inicio();
        while (adj != nullptr) {
            int u = adj->vertice;
            if (!visitado[u] && ehCiclicoUtil(u, visitado, pilha)) {
                return true;
            } else if (pilha[u]) {
                return true;
            }
            adj = adj->proximo;
        }
    }
    pilha[v] = false;
    return false;
}

// Função para verificar se o grafo é planar
void Grafo::ehPlanar() {
    bool testeEhplanar;
    if (numvertices <= 4) {
        testeEhplanar = true;
    }

    int numArestas = 0;
    for (int i = 0; i < numvertices; i++) {
        numArestas += listaAdjacencia[i].tamanho();
    }

    if (numArestas / 2 <= 3 * numvertices - 6) {
        testeEhplanar = true;
    }

    testeEhplanar = false;

    if (testeEhplanar){
      cout << "Grafo é um grafo planar. " << endl;
    } else {
      cout << "Grafo, não é um grafo planar. " << endl;
    }
}