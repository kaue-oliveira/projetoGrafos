// Arquivo grafo.cpp
#include "grafo.h"
#include "filadinamica.h"
#include "pilhadinamica.h"
#include <queue>
#include <fstream>
#include <iomanip>
#include <chrono> // Para medir o tempo
#include <stack>
#include <climits> //para usar INT_MAX
#include <vector>
#include <sstream> // Incluir a biblioteca <sstream> para usar o stringstream
#include <list>

const string Nomearquivo = "grafo.txt";
using namespace std::chrono;
using namespace std;

/*Construtor para o grafo, ele inicia a matriz e tambem ajusta as arestas que existem, ou coisas do tipo*/
Grafo::Grafo(int max, int nulo) 
{
    numvertices = 0;
    maxvertices = max;
    nulo = INT_MIN;
    arestanula = nulo;
    valorado = false;

    marcador = new bool[maxvertices]; //usado nas Buscas 

    vertices = new string[maxvertices]; 

    matrizadjacencias = new int *[maxvertices];
    for (int i=0 ; i<maxvertices ; i++){
        matrizadjacencias[i] = new int[maxvertices];
    }

    for (int i=0 ; i<maxvertices ; i++){
        for (int j=0 ; j<maxvertices ; j++){
            matrizadjacencias[i][j] = arestanula;
        }
    }
}

/*Destrutor para o grafo, ele vai desalocando linhas e colunas do grafo*/
// No destrutor, libere a memória alocada para a matriz marcador
Grafo::~Grafo() 
{
    delete [] vertices;
    for (int i=0 ; i<maxvertices ; i++){
        delete [] matrizadjacencias[i];
    }
    delete [] matrizadjacencias;
}
/*Essa função é bem util pra verificar se um vertice existe ou não, caso não exista é retornado -1
é importante mudar caso queira adicionar pesos nas arestas*/
int Grafo::obterindice(string item) 
{
    for (int i = 0; i < numvertices; i++) {
        if (vertices[i] == item) {
            return i;
        }
    }
    return -1;
}
/*Insere um vertice no grafo(util pra construir ou editar), não é possivel ter dois vertices com o mesmo nome*/
void Grafo::inserevertice(string item) 
{
  for (int i = 0; i < numvertices; i++){
    if (item == vertices[i]){
      cerr << "Ocorreu um erro pois esse vertice ja existe." << endl;
    }
  }
    vertices[numvertices] = item;
    numvertices++;
}

/*Insere um grafo no vertice(util pra construir ou editar)*/
void Grafo::inserearesta(string Nosaida, string Noentrada) //caso não seja valorado
{
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

void Grafo::inserearestaValorado(string Nosaida, string Noentrada, string peso) {
    int linha = obterindice(Nosaida);
    int coluna = obterindice(Noentrada);
    int pesoNovo = stoi(peso);
    if (linha != -1 and coluna != -1) {
        // Verifica se o peso é um número válido antes de atribuir à matriz de adjacências
        if (pesoNovo >= INT_MIN && pesoNovo <= INT_MAX) {
            if (direcionado) {
                matrizadjacencias[linha][coluna] = pesoNovo;
            } else {
                matrizadjacencias[linha][coluna] = pesoNovo;
                matrizadjacencias[coluna][linha] = pesoNovo;
            }
        } else {
            cerr << "Erro: Peso inválido na aresta (" << Nosaida << ", " << Noentrada << ", " << peso << "). A aresta será ignorada." << endl;
        }
    }
}

/*Essa função pode ser bem util futuramente, ele verifica se o grafo é direcionado ou não, e exibe 
na saida padrão o grau do vertice, ele usa for pra achar o grau e um contador pra ir somando o grau*/
void Grafo::obtergrau(string item) 
{
    int linha = obterindice(item);
    if (linha == -1) {
        cout << "O vertice " << item << " não existe no grafo." << endl;
        return;
    }

    if (!direcionado) {
        int grau = 0;
        // Contar o número de arestas de saída
        for (int i = 0; i < maxvertices; i++) {
            if (matrizadjacencias[linha][i] != arestanula) {
                grau++;
            }
        }
        // Contar o número de arestas de entrada
        for (int i = 0; i < maxvertices; i++) {
            if (matrizadjacencias[i][linha] != arestanula) {
                grau++;
            }
        }
        cout << "O vertice " << item << " tem grau " << grau << endl;
    } else {
        int saida = 0;
        int entrada = 0;
        // Contar o número de arestas de saída
        for (int i = 0; i < numvertices; i++) {
            if (matrizadjacencias[linha][i] != arestanula) {
                saida++;
            }
        }
        // Contar o número de arestas de entrada
        for (int i = 0; i < numvertices; i++) {
            if (matrizadjacencias[i][linha] != arestanula) {
                entrada++;
            }
        }
        cout << "O vertice " << item << " tem grau de saída " << saida
        << " e grau de entrada " << entrada << endl;
    }
}

/*O obtergrauaxu foi criado para que o grau de um vertice seja retornado
e não apenas exibido na saida padrão*/
int Grafo::obtergrauaux(string item) 
{
    int linha = obterindice(item);
    int grau = 0;
    if (linha != -1) {
        for (int i = 0; i < maxvertices; i++) {
            if (matrizadjacencias[linha][i] != arestanula) {
                grau++;
            }
        }
    }
    for (int i = 0; i < maxvertices; i++){
      if(matrizadjacencias[i][linha] != arestanula){
        grau++;
      }
    }
    return grau;    
}
/*Impressão da matriz de adjacências*/
void Grafo::imprimirmatriz() 
{
    cout << "Matriz de adjacências:\n";
    for (int i = 0; i < numvertices; i++) {
        for (int j = 0; j < numvertices; j++) {
            cout << matrizadjacencias[i][j] << " ";
        }
        cout << endl;
    }
}

/*É util pra imprimir o grafo como se estivesse na lista*/
void Grafo::imprimirvertices() 
{
    cout << "Lista de Vértices:\n";
    for (int i = 0; i < numvertices; i++) {
        cout << vertices[i];
        for (int j = 0; j < numvertices; j++){
            if (matrizadjacencias[i][j] != arestanula){
                cout << "->" << vertices[j];
            }
        }
        cout << endl;
    }
}
/*Para remover o vertice é necessario apagar ele e quem depende dele
as arestas são atualizadas para 0, essa linha e coluna são apagadas e o vetor
que guarda os vertices é atualizado(esse vetor existe pois facilita muito algumas aplicações)*/
void Grafo::removervertice(string verticeEditar)
{
    bool existe = false;
    int indiceVertice = -1;
    for (int i = 0; i < numvertices; i++) {
        if (vertices[i] == verticeEditar) {
            existe = true;
            indiceVertice = i;
            break;
        }
    }

    if (!existe) {
        cerr << "O vértice não existe =(" << endl;
    } else {

    int vertice = stoi(verticeEditar) - 1;

    // Atualizar a matriz de adjacências
    for (int i = vertice; i < numvertices - 1; i++) {
        for (int j = 0; j < numvertices; j++) {
            matrizadjacencias[i][j] = matrizadjacencias[i + 1][j];
        }
    }

    for (int j = vertice; j < numvertices - 1; j++) {
        for (int i = 0; i < numvertices; i++) {
            matrizadjacencias[i][j] = matrizadjacencias[i][j + 1];
        }
    }

    for (int i = 0; i < numvertices; i++) {
        matrizadjacencias[numvertices - 1][i] = arestanula;
        matrizadjacencias[i][numvertices - 1] = arestanula;
    }

    // Atualizar o array de vértices
    for (int i = indiceVertice; i < numvertices - 1; i++) {
        vertices[i] = vertices[i + 1];
    }
    numvertices--;

    cout << "Vértice removido com sucesso =)" << endl;
    reescreverArquivo();
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

    // Extrai a substring que contém as arestas
    size_t pos = arestas_pos + 6;
    size_t end_pos = line.find("};", pos);
    if (end_pos == string::npos) {
        cerr << "Erro de formatação: falta '};' encerrando as arestas." << endl;
        exit(EXIT_FAILURE);
    }

    string arestas_str = line.substr(pos, end_pos - pos);

    // Verifica se há valores associados às arestas (grafos valorados)
    size_t open_paren_pos = arestas_str.find('(');
    while (open_paren_pos != string::npos) {
        size_t close_paren_pos = arestas_str.find(')', open_paren_pos);
        if (close_paren_pos == string::npos) {
            cerr << "Erro de formatação: falta ')' encerrando uma aresta." << endl;
            exit(EXIT_FAILURE);
        }

        string aresta = arestas_str.substr(open_paren_pos + 1, close_paren_pos - open_paren_pos - 1);
        if (count(aresta.begin(), aresta.end(), ',') == 2) {
            return true; // Encontrou uma aresta com 3 elementos, indicando um grafo valorado
        }

        open_paren_pos = arestas_str.find('(', close_paren_pos);
    }

    return false;
}




/*Abre um arquivo e le o grafo, ele pega os vertices e
caso estejam no formato correto ele insere, caso não
ele apenas sai do programa.
a leitura de arestas usa um vetor pra salvar a primeira posição do grafo e a segunda
ele chama obterindice pra ver se os vertices existem*/
void Grafo::lergrafo() {
    ifstream file(Nomearquivo);

    if (!file.is_open()) {
        cerr << "Não foi possível abrir o arquivo " << Nomearquivo << "." << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    getline(file, line);

    // Verificando se a linha está no formato esperado
    size_t vertices_pos = line.find("V = {");
    size_t arestas_pos = line.find("}; A = {");

    if (vertices_pos == string::npos || arestas_pos == string::npos) {
        cerr << "Erro de formatação: falta {} encerrando os vértices ou arestas." << endl
             << "Por favor, garanta que o formato no arquivo está no seguinte padrão: V = {a,b,c,...}; A = {(a,b),(b,c),...}; ou V = {a,b,c,...}; A = {(a,b,1),(b,c,-2),...};" << endl;
        file.close();
        exit(EXIT_FAILURE);
    }

    bool isValorado = verifica(line);
    if (isValorado){
      valorado = true;
    } else {
      valorado = false;
    }

    // Extrai a substring que contém os vértices
    size_t pos = vertices_pos + 5;
    size_t end_pos = line.find("}", pos);
    string vertices_str = line.substr(pos, end_pos - pos);

    // Processa os vértices
    stringstream ss(vertices_str);
    string vertex;
    while (getline(ss, vertex, ',')) {
        inserevertice(vertex); // Não removemos espaços em branco nos vértices
    }

    // Se não há arestas, encerra a função aqui
    if (arestas_pos == string::npos) {
        file.close();
        return;
    }

    // Extrai e processa as arestas
    pos = arestas_pos + 6;
    end_pos = line.find("};", pos);
    string arestas_content = line.substr(pos, end_pos - pos);

    // Processa as arestas sem remover espaços em branco
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
                inserearestaValorado(no1, no2, valor); // Mantemos o valor como string
            } else {
                inserearesta(no1, no2);
            }
        }
    }

    file.close();
}




/*A quantidade total de vertices ja esta presente na criação do grafo
usamos isso nesse codigo pois facilita muito alguns metodos*/
int Grafo::qtdvertice()
{
  return(numvertices);
}
/*como o valor padrão para a ausencia de aresta nesse codigo é 0
se usa dois for e um contador pra ver o que na matriz não tem 0*/
int Grafo::qtdarestas()
{
  int contadorArestas = 0;
  for (int i = 0; i < numvertices; i++){
    for (int j = 0; j < numvertices; j++){
      if (matrizadjacencias[i][j] != arestanula){
          contadorArestas++;
      }
    }
  }
  return(contadorArestas);
}
/**/
void Grafo::reescreverArquivo() {
    ofstream arquivo(Nomearquivo);

    if (!arquivo.is_open()) {
        cerr << "Não foi possível abrir o arquivo " << Nomearquivo << "." << endl;
        return;
    }

    // Escreve os vértices
    arquivo << "V = {";
    for (int i = 0; i < numvertices; i++) {
        arquivo << vertices[i];
        if (i != numvertices - 1) {
            arquivo << ",";
        }
    }
    arquivo << "}";

    // Verifica se o grafo é valorado
    bool isValorado = false;
    for (int i = 0; i < numvertices && !isValorado; i++) {
        for (int j = 0; j < numvertices; j++) {
            if (matrizadjacencias[i][j] != arestanula && matrizadjacencias[i][j] != 1) {
                isValorado = true;
                break;
            }
        }
    }

    // Escreve as arestas
    arquivo << "; A = {";
    bool temArestas = false; // Verifica se há alguma aresta
    for (int i = 0; i < numvertices; i++) {
        for (int j = 0; j < numvertices; j++) {
            if (matrizadjacencias[i][j] != arestanula) {
                temArestas = true;
                arquivo << "(" << vertices[i] << "," << vertices[j];
                if (isValorado) {
                    arquivo << "," << matrizadjacencias[i][j];
                }
                arquivo << "),";
            }
        }
    }

    if (temArestas) {
        // Remove a vírgula extra no final
        arquivo.seekp(-1, ios_base::end);
    }
    arquivo << "};";

    arquivo.close();
}

void Grafo::limpamarcador()
{
    for (int i = 0; i<maxvertices ; i++){
    marcador[i] = false;    
    }   
}

void Grafo::buscaemlarguraArvore() {
    if (ehConexo()) {
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

                for (int v = 0; v < numvertices; ++v) {
                    if (matrizadjacencias[u][v] != arestanula && !visitado[v]) {
                        fila.push(v);
                        visitado[v] = true;
                        distancia[v] = distancia[u] + 1;
                        pais[v] = u;
                    }
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
            if (pais[i] != -1) {
                cout << "(" << vertices[pais[i]] << ", " << vertices[i] << ") - Distância: " << distancia[i] << "\n";
            }
        }
    } else {
        cerr << "Grafo não é conexo, não é possível executar busca em largura." << endl;
    }
}



void Grafo::buscaemprofundidadeArvore() {
    if (ehConexo()) {
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

                for (int v = 0; v < numvertices; ++v) {
                    if (matrizadjacencias[u][v] != arestanula && !visitado[v]) {
                        pilha.push(v);
                        visitado[v] = true;
                        distancia[v] = distancia[u] + 1;
                        pais[v] = u;
                    }
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
            if (pais[i] != -1) {
                cout << "(" << vertices[pais[i]] << ", " << vertices[i] << ") - Distância: " << distancia[i] << "\n";
            }
        }
    } else {
        cerr << "Grafo não é conexo, não é possível executar busca em profundidade." << endl;
    }
}

void Grafo::buscaemlargura(string origem, string destino) {
    if (direcionado) {
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
                for (int i = 0; i < maxvertices; i++) {
                    if (matrizadjacencias[indice][i] != arestanula) {
                        if (!marcador[i]) {
                            cout << "Enfileirando: " << vertices[i] << endl;
                            filaVertices.inserir(vertices[i]);
                            marcador[i] = true;
                        }
                    }
                }
                // No caso de grafos direcionados, verifique também as arestas de entrada
                for (int i = 0; i < maxvertices; i++) {
                    if (matrizadjacencias[i][indice] != arestanula) {
                        if (!marcador[i]) {
                            cout << "Enfileirando (aresta de entrada): " << vertices[i] << endl;
                            filaVertices.inserir(vertices[i]);
                            marcador[i] = true;
                        }
                    }
                }
            }
        } while (!filaVertices.estavazio() && !encontrado);
        if (!encontrado) {
            cout << "Caminho não encontrado!\n";
        }
    } else {
filadinamica filavertices;
        bool encontrado = false;
        limpamarcador();
        filavertices.inserir(origem);
        do{
            string verticeatual = filavertices.remover();
            if (verticeatual == destino){
                cout << "Visitando: " << verticeatual << endl;
                cout << "Caminho encontrado!\n";
                encontrado = true;
            } else{
                int indice = obterindice(verticeatual);
                cout << "Visitando: " << verticeatual << endl;
                for (int i=0 ; i<maxvertices ; i++){
                    if (matrizadjacencias[indice][i] != arestanula){
                        if (!marcador[i]){
                            cout << "Enfileirando: " << vertices[i] << endl;
                            filavertices.inserir(vertices[i]);
                            marcador[i] = true;
                        }
                    }  
                }
            }
        } while (!filavertices.estavazio() && !encontrado);
        if (!encontrado){
            cout << "Caminho nao encontrado!\n";
        }    }
}

void Grafo::buscaemprofundidade(string origem, string destino) {
    if (direcionado) {
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
                for (int i = 0; i < maxvertices; i++) {
                    if (matrizadjacencias[indice][i] != arestanula) {
                        if (!marcador[i]) {
                            cout << "Empilhando: " << vertices[i] << endl;
                            pilhaVertices.inserir(vertices[i]);
                            marcador[i] = true;
                        }
                    }
                }
                // No caso de grafos direcionados, verifique também as arestas de entrada
                for (int i = 0; i < maxvertices; i++) {
                    if (matrizadjacencias[i][indice] != arestanula) {
                        if (!marcador[i]) {
                            cout << "Empilhando (aresta de entrada): " << vertices[i] << endl;
                            pilhaVertices.inserir(vertices[i]);
                            marcador[i] = true;
                        }
                    }
                }
            }
        } while (!pilhaVertices.estavazio() && !encontrado);
        if (!encontrado) {
            cout << "Caminho não encontrado!\n";
        }
    } else {
pilhadinamica pilhavertices;
        bool encontrado = false;
        limpamarcador();
        pilhavertices.inserir(origem);
        do{
            string verticeatual = pilhavertices.remover();
            if (verticeatual == destino){
                cout << "Visitando: " << verticeatual << endl;
                cout << "Caminho encontrado!\n";
                encontrado = true;
            } else{
                int indice = obterindice(verticeatual);
                cout << "Visitando: " << verticeatual << endl;
                for (int i=0 ; i<maxvertices ; i++){
                    if (matrizadjacencias[indice][i] != arestanula){
                        if (!marcador[i]){
                            cout << "Empilhando: " << vertices[i] << endl;
                            pilhavertices.inserir(vertices[i]);
                            marcador[i] = true;
                        }
                    }  
                }
            }
        } while (!pilhavertices.estavazio() && !encontrado);
        if (!encontrado){
            cout << "Caminho nao encontrado!\n";
        }    }
}
void Grafo::removeraresta(string origem, string destino)
{
    if (direcionado){
    int verticeorigem = obterindice(origem);
    int verticedestino = obterindice(destino);
    
    // Verifica se os vértices são válidos
    if (verticeorigem >= 0 and verticeorigem < numvertices and verticedestino >= 0 and verticedestino < numvertices) {
        matrizadjacencias[verticeorigem][verticedestino] = arestanula;
        // Se o grafo for não direcionado, descomente a linha abaixo
        // matrizadjacencias[verticedestino][verticeorigem] = 0;
    }
    reescreverArquivo();
    } else {
    int verticeorigem = obterindice(origem);
    int verticedestino = obterindice(destino);
    
    // Verifica se os vértices são válidos
    if (verticeorigem >= 0 and verticeorigem < numvertices and verticedestino >= 0 and verticedestino < numvertices) {
        matrizadjacencias[verticeorigem][verticedestino] = arestanula;
        // Se o grafo for não direcionado, descomente a linha abaixo
        matrizadjacencias[verticedestino][verticeorigem] = arestanula;
    }
    }
}
bool Grafo::ehConexo() {
    if (numvertices == 0) {
        return true; // Um grafo vazio pode ser considerado conexo
    }

    // Inicializa o vetor de visitados
    bool *visitados = new bool[numvertices];
    for (int i = 0; i < numvertices; i++) {
        visitados[i] = false;
    }

    // Fila para a BFS
    queue<int> fila;

    // Começa a BFS a partir do primeiro vértice
    for (int inicio = 0; inicio < numvertices; inicio++) {
        if (!visitados[inicio]) {
            fila.push(inicio);
            visitados[inicio] = true;

            while (!fila.empty()) {
                int verticeAtual = fila.front();
                fila.pop();

                for (int i = 0; i < numvertices; i++) {
                    if (matrizadjacencias[verticeAtual][i] != arestanula && !visitados[i]) {
                        fila.push(i);
                        visitados[i] = true;
                    }
                }
            }
        }
    }

    // Verifica se todos os vértices foram visitados
    bool conexo = true;
    for (int i = 0; i < numvertices; i++) {
        if (!visitados[i]) {
            conexo = false;
            break;
        }
    }

    delete[] visitados;

    return conexo;
}


void Grafo::possuiciclos()
{
  int contador = 0;
  for (int i = 0; i < numvertices; i++){
      if (matrizadjacencias[i][i] != arestanula){
        contador ++;
      }
  }

  cout << "O grafo possui " << contador << " ciclos" << endl;
}

void Grafo::eheuleriano()
{
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
    // Inicializa um array para marcar os vértices visitados
    bool* visitado = new bool[numvertices];
    for (int i = 0; i < numvertices; ++i) {
        visitado[i] = false;
    }

    // Função auxiliar para realizar uma DFS a partir de um vértice
    auto dfs = [&](int vertice) {
        stack<int> pilha;
        pilha.push(vertice);

        while (!pilha.empty()) {
            int u = pilha.top();
            pilha.pop();

            if (!visitado[u]) {
                visitado[u] = true;

                // Percorre todos os vértices adjacentes ao vértice atual
                for (int i = 0; i < numvertices; i++) {
                    // Se houver uma aresta entre os vértices e o vértice adjacente não foi visitado
                    if (matrizadjacencias[u][i] != arestanula && !visitado[i]) {
                        // Insere o vértice adjacente na pilha para visita posterior
                        pilha.push(i);
                    }
                }
            }
        }
    };

    // Realiza uma DFS a partir do primeiro vértice
    dfs(0);

    // Verifica se todos os vértices foram visitados na primeira DFS
    bool fortementeConexo = true;
    for (int i = 0; i < numvertices; ++i) {
        if (!visitado[i]) {
            fortementeConexo = false;
            break;
        }
    }

    // Se o grafo não for fortemente conexo, não há necessidade de continuar
    if (!fortementeConexo) {
        delete[] visitado;
        return false;
    }

    // Reutiliza o array de visitados para realizar outra DFS a partir de um vértice diferente
    for (int i = 0; i < numvertices; ++i) {
        visitado[i] = false;
    }

    // Realiza uma segunda DFS a partir de um vértice diferente
    dfs(1);

    // Verifica se todos os vértices foram visitados na segunda DFS
    for (int i = 0; i < numvertices; ++i) {
        if (!visitado[i]) {
            fortementeConexo = false;
            break;
        }
    }

    delete[] visitado;

    return fortementeConexo;
}


/*Verifica existencia do vertice para inserir ele caso o 
vertice não exista*/
void Grafo::verificaExistencia(string vertice)
{
  bool teste = true;
  for (int i = 0; i < numvertices; i++){
    if (vertice == vertices[i]){
      teste = false;
    }
  }

  if (teste){
    inserevertice(vertice);
  } else {
    cerr << "Esse nome de vertice ja existe, tente novamente" << endl;
  }
}


void Grafo::verificaExistenciaAresta(string saida, string entrada, string peso)
{
  if (valorado == false){
  bool testeSaida = false;
  bool testeEntrada = true;
  for (int i = 0; i < numvertices; i++){
    if (vertices[i] == saida){
      testeSaida = true;
    }
    if (vertices[i] == entrada){
      testeEntrada = true;
    }
  }
  if (testeEntrada and testeSaida == true){
    inserearesta(saida, entrada);
  } else {
    cerr << "Um dos vertices de entrada ou saida, não existe" << endl;
  }
  } else {
   bool testeSaida = false;
  bool testeEntrada = true;
  for (int i = 0; i < numvertices; i++){
    if (vertices[i] == saida){
      testeSaida = true;
    }
    if (vertices[i] == entrada){
      testeEntrada = true;
    }
  }
  if (testeEntrada and testeSaida == true){
    inserearestaValorado(saida, entrada, peso);
  } else {
    cerr << "Um dos vertices de entrada ou saida, não existe" << endl;
  } 
  }
}
void Grafo::prim() {
    // Verifica se o grafo é conexo
    if (ehConexo()) {
        vector<int> pais(numvertices, -1);
        vector<int> distancia(numvertices, INT_MAX); // Distância em termos de peso da aresta
        vector<bool> naArvore(numvertices, false);

        auto executarPrim = [&](int inicio) {
            distancia[inicio] = 0;
            pais[inicio] = -1;

            // Min-heap para armazenar vértices que estão sendo processados
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> filaPrioridade;

            filaPrioridade.push(make_pair(distancia[inicio], inicio));

            // Loop principal do algoritmo de Prim
            while (!filaPrioridade.empty()) {
                int u = filaPrioridade.top().second;
                filaPrioridade.pop();

                if (naArvore[u]) continue; // Se já está na árvore, pular

                naArvore[u] = true;

                for (int v = 0; v < numvertices; ++v) {
                    int peso = matrizadjacencias[u][v];
                    if (peso != arestanula && !naArvore[v] && peso < distancia[v]) {
                        pais[v] = u;
                        distancia[v] = peso;
                        filaPrioridade.push(make_pair(distancia[v], v));
                    }
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
                cout << "(" << vertices[pais[i]] << ", " << vertices[i] << ") - Peso: " << matrizadjacencias[pais[i]][i] << "\n";
            }
        }
    } else {
        cerr << "Grafo não é conexo, logo não é possível executar Prim." << endl;
    }
}



