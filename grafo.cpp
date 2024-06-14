
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

// Inicialização da classe Grafo
Grafo::Grafo(int max, int nulo) 
{
    cout << "Inicializando o grafo..." << endl;
    numvertices = 0;
    maxvertices = max;
    arestanula = nulo;
    valorado = false;
    direcionado = false;
    vertices = new string[maxvertices];
    marcador = new bool[maxvertices];
    listaAdjacencia = new Lista[maxvertices];
    cout << "Grafo inicializado com " << maxvertices << " vértices possíveis e aresta nula " << arestanula << endl;
}

Grafo::~Grafo() 
{
    cout << "Destruindo o grafo..." << endl;
    delete[] vertices;
    delete[] marcador;
    delete[] listaAdjacencia;
    cout << "Grafo destruído." << endl;
}

void Grafo::defineDirecionado(string resposta) {
    if (resposta == "y" || resposta == "Y") {
        direcionado = true;
        cout << "O grafo foi definido como direcionado." << endl;
    } else {
        direcionado = false;
        cout << "O grafo foi definido como não direcionado." << endl;
    }
}

void Grafo::inserearesta(string Nosaida, string Noentrada) 
{
    int linha = obterindice(Nosaida);
    int coluna = obterindice(Noentrada);

    if (linha != -1 and coluna != -1) {
        listaAdjacencia[linha].inserir(coluna, 1);
        cout << "Aresta inserida de " << Nosaida << " para " << Noentrada << endl;
        if (!direcionado) {
            listaAdjacencia[coluna].inserir(linha, 1);
            cout << "Aresta inserida de " << Noentrada << " para " << Nosaida << " (não direcionado)." << endl;
        }
    } else {
        cerr << "Erro ao inserir aresta: vértice não encontrado." << endl;
    }
}

void Grafo::inserearestaValorado(string Nosaida, string Noentrada, string peso) 
{
    int linha = obterindice(Nosaida);
    int coluna = obterindice(Noentrada);
    int pesoNovo = stoi(peso);

    if (linha != -1 and coluna != -1) {
        listaAdjacencia[linha].inserir(coluna, pesoNovo);
        cout << "Aresta valorada inserida de " << Nosaida << " para " << Noentrada << " com peso " << pesoNovo << endl;
        if (!direcionado) {
            listaAdjacencia[coluna].inserir(linha, pesoNovo);
            cout << "Aresta valorada inserida de " << Noentrada << " para " << Nosaida << " com peso " << pesoNovo << " (não direcionado)" << endl;
        }
    } else {
        cerr << "Erro ao inserir aresta valorada: vértice não encontrado." << endl;
    }
}

void Grafo::inserevertice(string item) {
    if (obterindice(item) != -1) {
        cerr << "Erro: o vértice " << item << " já existe." << endl;
        return;
    }
    vertices[numvertices] = item;
    numvertices++;
    cout << "Vértice " << item << " inserido com sucesso." << endl;
}

void Grafo::removervertice(string verticeEditar) {
    int indiceVertice = obterindice(verticeEditar);
    if (indiceVertice == -1) {
        cerr << "Erro: o vértice " << verticeEditar << " não existe." << endl;
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
    cout << "Vértice " << verticeEditar << " removido com sucesso." << endl;
    reescreverArquivo();
}

void Grafo::removeraresta(string origem, string destino) {
    int verticeorigem = obterindice(origem);
    int verticedestino = obterindice(destino);
    if (verticeorigem == -1 || verticedestino == -1) {
        cerr << "Erro ao remover aresta: um dos vértices não existe." << endl;
        return;
    }

    listaAdjacencia[verticeorigem].remover(verticedestino);
    cout << "Aresta de " << origem << " para " << destino << " removida." << endl;

    if (!direcionado) {
        listaAdjacencia[verticedestino].remover(verticeorigem);
        cout << "Aresta de " << destino << " para " << origem << " removida (não direcionado)." << endl;
    }
    reescreverArquivo();
}

// Manipulação de arquivos

void Grafo::lergrafo() {
    ifstream file(Nomearquivo);
    if (!file.is_open()) {
        cerr << "Erro: não foi possível abrir o arquivo " << Nomearquivo << endl;
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
    cout << "Grafo lido com sucesso do arquivo " << Nomearquivo << endl;
    file.close();
}

void Grafo::reescreverArquivo() 
{
    ofstream arquivo(Nomearquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro: não foi possível abrir o arquivo " << Nomearquivo << endl;
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
    cout << "Arquivo " << Nomearquivo << " atualizado com sucesso." << endl;
    arquivo.close();
}

bool Grafo::verifica(const string& line) 
{
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

// Funções úteis na manipulação do grafo

int Grafo::obterindice(string item) 
{
    for (int i = 0; i < numvertices; i++) {
        if (vertices[i] == item) {
            return i;
        }
    }
    return -1;
}

void Grafo::obtergrau(string item) 
{
    int linha = obterindice(item);
    if (linha == -1) {
        cerr << "O vértice " << item << " não existe no grafo." << endl;
         
    }

    if (!direcionado) {
        int grau = listaAdjacencia[linha].tamanho();
        cout << "O vértice " << item << " tem grau " << grau << endl;
    } else {
        int saida = listaAdjacencia[linha].tamanho();
        int entrada = 0;
        for (int i = 0; i < numvertices; i++) {
            entrada += listaAdjacencia[i].contar(linha);
        }
        cout << "O vértice " << item << " tem grau de saída " << saida << " e grau de entrada " << entrada << endl;
    }
}

int Grafo::obtergrauaux(string item) 
{
    int linha = obterindice(item);
    if (linha == -1) {
        return 0;
    }
    return listaAdjacencia[linha].tamanho();
}

bool Grafo::ehConexo() 
{
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

bool Grafo::ehdirecionado() 
{
    return direcionado;
}

bool Grafo::ehvalorado() 
{
    return valorado;
}

int Grafo::qtdvertice() 
{
    return numvertices;
}

int Grafo::qtdarestas() 
{
    int contadorArestas = 0;
    for (int i = 0; i < numvertices; i++) {
        contadorArestas += listaAdjacencia[i].tamanho();
    }
    return contadorArestas;
}

void Grafo::verificaExistencia(string vertice) 
{
    bool teste = true;
    for (int i = 0; i < numvertices; i++) {
        if (vertice == vertices[i]) {
            teste = false;
        }
    }

    if (teste) {
        inserevertice(vertice);
    } else {
        cerr << "Erro: o vértice " << vertice << " já existe. Tente novamente." << endl;
    }
}

void Grafo::verificaExistenciaAresta(string saida, string entrada, string peso) 
{
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
        if (testeEntrada and testeSaida) {
            inserearesta(saida, entrada);
        } else {
            cerr << "Erro: um dos vértices de entrada ou saída não existe." << endl;
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
        if (testeEntrada and testeSaida) {
            inserearestaValorado(saida, entrada, peso);
        } else {
            cerr << "Erro: um dos vértices de entrada ou saída não existe." << endl;
        }
    }
}

int Grafo::obtergrauEntrada(std::string item) 
{
    int indice = obterindice(item);
    int grauEntrada = 0;
    for (int i = 0; i < numvertices; ++i) {
        if (listaAdjacencia[i].contar(indice) > 0) {
            grauEntrada++;
        }
    }
    return grauEntrada;
}

int Grafo::obtergrauSaida(string item) 
{
    int indice = obterindice(item);
    return listaAdjacencia[indice].tamanho();
}

// Funções de exibição

void Grafo::imprimirmatriz() 
{
    cout << "Função removida, pois a matriz de adjacências não é mais usada." << endl;
}

void Grafo::imprimirvertices() 
{
    cout << "Lista de Vértices:" << endl;
    for (int i = 0; i < numvertices; i++) {
        cout << vertices[i];
        listaAdjacencia[i].imprimir(vertices);
        cout << endl;
    }
}

void Grafo::possuiCiclos() 
{
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
    cout << "O grafo possui " << contador << " ciclos." << endl;
}

void Grafo::eheuleriano() 
{
    int grauImpar = 0;
    for (int i = 0; i < numvertices; i++) {
        int grauEntrada = obtergrauEntrada(vertices[i]);
        int grauSaida = obtergrauSaida(vertices[i]);
        if (grauEntrada != grauSaida) {
            grauImpar++;
        }
    }
    if (grauImpar == 0) {
        cout << "Grafo é euleriano." << endl;
    } else if (grauImpar == 2) {
        cout << "Grafo é semieuleriano." << endl;
    } else {
        cout << "Grafo não é euleriano." << endl;
    }
}

bool Grafo::eheulerianoAux() 
{
    int grauImpar = 0;
    for (int i = 0; i < numvertices; i++) {
        int grauEntrada = obtergrauEntrada(vertices[i]);
        int grauSaida = obtergrauSaida(vertices[i]);
        if (grauEntrada != grauSaida) {
            grauImpar++;
        }
    }
    return (grauImpar == 0 or grauImpar == 2);
}

bool Grafo::ehfortementeconexo() 
{
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

void Grafo::limpamarcador() 
{
    fill(marcador, marcador + maxvertices, false);
    cout << "Marcadores de vértices limpos." << endl;
}



void Grafo::buscaemlarguraArvore() 
{
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

    cout << "Árvore de busca em largura:" << endl;
    for (int i = 0; i < numvertices; ++i) {
    if (pais[i] != -1 || (pais[i] == -1 and listaAdjacencia[i].tamanho() > 0)) {
        cout << "(" << (pais[i] != -1 ? vertices[pais[i]] : vertices[i]) << ", " << vertices[i] << ") - Distância: " << distancia[i] << "\n";
    } else {
        cout << "(" << vertices[i] << ") - Isolado" << endl;
    }
}


    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end_time - start_time);
    cout << "Tempo total de execução: " << duration.count() << " segundos" << endl;
}

void Grafo::buscaemprofundidadeArvore() 
{
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

    cout << "Árvore de busca em profundidade:" << endl;
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
        cout << "Caminho não encontrado!" << endl;
    }
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end_time - start_time);
    cout << "Tempo total de execução: " << duration.count() << " segundos" << endl;
}
void Grafo::prim() {
    if (ehConexo()) {
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
                        cout << "Atualizando distância do vértice: " << vertices[v] << " para " << distancia[v] << endl;
                    }
                    adj = adj->proximo;
                }
            }
        };

        for (int i = 0; i < numvertices; i++) {
            if (!naArvore[i]) {
                cout << "Iniciando algoritmo de Prim a partir do vértice: " << vertices[i] << endl;
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
        cerr << "Grafo não é conexo, logo não é possível executar Prim. " << endl;
    }
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
            cout << vertices[caminho[i]] << " ";
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

    for (int i = 0; i < ordem.size(); i++) {
        cout << vertices[ordem[i]];
        if (i != ordem.size() - 1) {
            cout << " -> ";
        }
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
        cout << vertices[caminho[i]];
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
    Grafo fechamento(numvertices, /*aresta nula*/ {});
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
    if (eheulerianoAux()) {
        cout << "Começando a busca pelo caminho euleriano..." << endl;
        cout << "OBS: Se o grafo for grande isso pode levar um tempo =(" << endl;

        stack<string> caminho;
        stack<string> caminhoAux;
        vector<vector<bool>> visitado(numvertices, vector<bool>(numvertices, false));
        string verticeAtual = vertices[0];
        int indiceAtual = obterindice(verticeAtual);

        caminho.push(verticeAtual);

        while (!caminho.empty()) {
            verticeAtual = caminho.top();
            indiceAtual = obterindice(verticeAtual);
            bool encontrouAresta = false;

            for (int i = 0; i < numvertices; ++i) {
                if (listaAdjacencia[indiceAtual].contar(i) > 0 && !visitado[indiceAtual][i]) {
                    visitado[indiceAtual][i] = true;
                    visitado[i][indiceAtual] = true;
                    caminho.push(vertices[i]);
                    encontrouAresta = true;
                    break;
                }
            }

            if (!encontrouAresta) {
                caminhoAux.push(caminho.top());
                caminho.pop();
            }
        }

        cout << "Caminho Euleriano: ";
        while (!caminhoAux.empty()) {
            cout << caminhoAux.top();
            caminhoAux.pop();
            if (!caminhoAux.empty()) {
                cout << " -> ";
            }
        }
        cout << endl;
    } else {
        cerr << "Grafo não é euleriano, logo não tem caminho euleriano." << endl;
    }
}

void Grafo::tarjanPontesUtil(int u, vector<int>& disc, vector<int>& low, vector<int>& pai, vector<bool>& pontes, int& tempo, vector<pair<int, int>>& listaPontes) {
    disc[u] = low[u] = ++tempo;

    Lista::No* adj = listaAdjacencia[u].inicio();
    while (adj != nullptr) {
        int v = adj->vertice;
        if (disc[v] == -1) {
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
    if (testeBipartido) {
        cout << "Grafo é Bipartido. " << endl;
    } else {
        cout << "Este grafo não é bipartido. " << endl;
    }
}

bool Grafo::ehHamiltoniano() {
    vector<int> caminho(numvertices, -1);
    vector<bool> visitado(numvertices, false);

    caminho[0] = 0;
    visitado[0] = true;

    return hamiltonianUtil(1, caminho, visitado);
}

bool Grafo::hamiltonianUtil(int pos, vector<int>& caminho, vector<bool>& visitado) {
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


void Grafo::ehCiclico() {
    vector<bool> visitado(numvertices, false);
    vector<bool> pilha(numvertices, false);
    bool ciclico = false;

    for (int i = 0; i < numvertices; i++) {
        if (!visitado[i]) {
            if (ehCiclicoUtil(i, visitado, pilha)) {
                ciclico = true;
                break;
            }
        }
    }

    if (ciclico) {
        cout << "O grafo é cíclico." << endl;
    } else {
        cout << "O grafo não é cíclico." << endl;
    }
}



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

