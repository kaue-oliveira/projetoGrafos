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
const string Nomearquivo = "grafo.txt";
using namespace std::chrono;

/*Construtor para o grafo, ele inicia a matriz e tambem ajusta as arestas que existem, ou coisas do tipo*/
Grafo::Grafo(int max) 
{
    numvertices = 0;
    maxvertices = max;
    arestanula = 0;

    marcador = new bool[maxvertices]; //usado nas Buscas 

    vertices = new string[maxvertices]; 

    matrizadjacencias = new unsigned*[maxvertices];
    for (int i=0 ; i<maxvertices ; i++){
        matrizadjacencias[i] = new unsigned[maxvertices];
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
void Grafo::inserearesta(string Nosaida, string Noentrada) 
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
            if (matrizadjacencias[i][j] != 0){
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
        matrizadjacencias[numvertices - 1][i] = 0;
        matrizadjacencias[i][numvertices - 1] = 0;
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



/*Abre um arquivo e le o grafo, ele pega os vertices e
caso estejam no formato correto ele insere, caso não
ele apenas sai do programa.
a leitura de arestas usa um vetor pra salvar a primeira posição do grafo e a segunda
ele chama obterindice pra ver se os vertices existem*/
void Grafo::lergrafo() 
{
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

    if (vertices_pos == string::npos or arestas_pos == string::npos) {
        cerr << "Erro de formatação: falta {} encerrando os vértices ou arestas." << endl
             << "Por favor, garanta que o formato no arquivo está no seguinte padrão: V = {1,2,...}; A = {(1,2),(2,3),...};" << endl;
        file.close();
        // Saindo do programa com status de falha
        exit(EXIT_FAILURE);
    }

    // Extrai a substring que contém os vértices
    size_t pos = vertices_pos + 5; // Encontra o início dos vértices
    size_t end_pos = arestas_pos;
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

    // Se não há arestas, encerra a função aqui
    if (arestas_pos == string::npos) {
        file.close();
        return;
    }

    // Processa as arestas
    pos = arestas_pos + 6; // Encontra o início das arestas
    end_pos = line.find("};");
    string arestas_content = line.substr(pos, end_pos - pos);

    string aresta;
    string nos[2];
    int peso = 1; // Assumindo peso padrão de 1 para todas as arestas
    for (size_t i = 0; i <= arestas_content.size(); i++) {
        if (arestas_content[i] == '(') {
            i++; // Ignora o '('
            while (arestas_content[i] != ')') {
                if (isdigit(arestas_content[i])) {
                    aresta += arestas_content[i];
                }
                else if (!aresta.empty()) {
                    nos[0] = aresta;
                    aresta.clear();
                }
                i++;
            }
            if (!nos[0].empty()) {
                i++; // Pula o ','
                while (isdigit(arestas_content[i])) {
                    aresta += arestas_content[i];
                    i++;
                }
                nos[1] = aresta;

                // Verifica se os vértices existem antes de adicionar a aresta
                int indice1 = obterindice(nos[0]);
                int indice2 = obterindice(nos[1]);
                if (indice1 != -1 and indice2 != -1) {
                    inserearesta(nos[0], nos[1]);
                } else {
                    cerr << "Erro: Aresta inválida, vértice não encontrado." << endl
                         << "Por favor, garanta que o formato no arquivo está no seguinte padrão: V = {1,2,...}; A = {(1,2),(2,3),...};" << endl;
                    file.close();
                    // Saindo do programa com status de falha
                    exit(EXIT_FAILURE);
                }
                aresta.clear();
                nos[0].clear();
                nos[1].clear();
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
      if (matrizadjacencias[i][j] != 0){
          contadorArestas++;
      }
    }
  }
  return(contadorArestas);
}
/**/
void Grafo::reescreverArquivo()
{
    ofstream arquivo(Nomearquivo);

    // Escreve os vértices
    arquivo << "V = {";
    if (numvertices > 0) {
        for (int i = 0; i < numvertices; i++) {
            if (i != numvertices - 1) {
                arquivo << vertices[i] << ",";
            } else {
                arquivo << vertices[i] << "}";
            }
        }
    } else {
        arquivo << "}";
    }

    // Escreve as arestas
    arquivo << "; A = {";
    bool temArestas = false; // Verifica se há alguma aresta
    for (int i = 0; i < numvertices; i++) {
        for (int j = 0; j < numvertices; j++) {
            if (matrizadjacencias[i][j] != 0) {
                temArestas = true;
                arquivo << "(" << vertices[i] << "," << vertices[j] << "),";
            }
        }
    }

    if (!temArestas) {
        arquivo << "}";
    } else {
        // Remove a vírgula extra no final, se houver
        arquivo.seekp(-1, ios_base::end);
        arquivo << "}";
    }

    arquivo.close();
}

void Grafo::limpamarcador()
{
    if (marcador != nullptr) {
        for (int i = 0; i < maxvertices; i++)
         {
            marcador[i] = false;
        }
    } else {
        cerr << "Erro: Ponteiro marcador não inicializado." << endl;
    }
}

void Grafo::buscaemlargura(string origem, string destino) 
{
    bool torigem = false;
    bool tdestino = false;
    for (int i = 0; i < numvertices; i++) {
        if (origem == vertices[i]) {
            torigem = true;
        }
        if (destino == vertices[i]) {
            tdestino = true;
        }
    }
    if (tdestino and torigem) {
        filadinamica filavertices;
        bool encontrado = false;
        limpamarcador();

        int indiceOrigem = obterindice(origem);
        filavertices.inserir(origem);
        marcador[indiceOrigem] = true;

        auto start_time = high_resolution_clock::now();

        while (!filavertices.estavazio() and !encontrado) {
            string verticeatual = filavertices.primeiroFila();
            filavertices.remover();
            cout << "Visitando: " << verticeatual << endl;

            if (verticeatual == destino) {
                cout << "Caminho encontrado!\n";
                encontrado = true;
            } else {
                int indice = obterindice(verticeatual);
                for (int i = 0; i < maxvertices; i++) {
                    if (matrizadjacencias[indice][i] != arestanula and !marcador[i]) {
                        cout << "Enfileirando: " << vertices[i] << endl;
                        filavertices.inserir(vertices[i]);
                        marcador[i] = true;
                    }
                }
            }
        }

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(end_time - start_time);

        cout << "Tempo total de execução: " << duration.count() << " segundos" << endl;

        if (!encontrado) {
            cout << "Caminho não encontrado!\n";
        }
    } else {
        cerr << "Erro: Um dos vértices não existe.\n";
    }
}

void Grafo::buscaemprofundidade(string origem, string destino) 
{
    bool torigem = false;
    bool tdestino = false;
    for (int i = 0; i < numvertices; i++) {
        if (origem == vertices[i]) {
            torigem = true;
        }
        if (destino == vertices[i]) {
            tdestino = true;
        }
    }
    if (tdestino and torigem) {
        pilhadinamica pilhavertices;
        bool encontrado = false;
        limpamarcador();

        int indiceOrigem = obterindice(origem);
        pilhavertices.inserir(origem);
        marcador[indiceOrigem] = true;

        auto start_time = high_resolution_clock::now();

        while (!pilhavertices.estavazio() and !encontrado) {
            string verticeatual = pilhavertices.topo();
            pilhavertices.remover();
            cout << "Visitando: " << verticeatual << endl;

            if (verticeatual == destino) {
                cout << "Caminho encontrado!\n";
                encontrado = true;
            } else {
                int indice = obterindice(verticeatual);
                for (int i = 0; i < maxvertices; i++) {
                    if (matrizadjacencias[indice][i] != arestanula and !marcador[i]) {
                        cout << "Empilhando: " << vertices[i] << endl;
                        pilhavertices.inserir(vertices[i]);
                        marcador[i] = true;
                    }
                }
            }
        }

        auto end_time = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(end_time - start_time);

        cout << "Tempo total de execução: " << duration.count() << " segundos" << endl;

        if (!encontrado) {
            cout << "Caminho não encontrado!\n";
        }
    } else {
        cerr << "Erro: Um dos vértices não existe.\n";
    }
}

void Grafo::removeraresta(string origem, string destino)
{
    if (direcionado){
    int verticeorigem = stoi(origem) - 1;
    int verticedestino = stoi(destino) - 1;
    
    // Verifica se os vértices são válidos
    if (verticeorigem >= 0 and verticeorigem < numvertices and verticedestino >= 0 and verticedestino < numvertices) {
        matrizadjacencias[verticeorigem][verticedestino] = 0;
        // Se o grafo for não direcionado, descomente a linha abaixo
        // matrizadjacencias[verticedestino][verticeorigem] = 0;
    }
    reescreverArquivo();
    } else {
    int verticeorigem = stoi(origem) - 1;
    int verticedestino = stoi(destino) - 1;
    
    // Verifica se os vértices são válidos
    if (verticeorigem >= 0 and verticeorigem < numvertices and verticedestino >= 0 and verticedestino < numvertices) {
        matrizadjacencias[verticeorigem][verticedestino] = 0;
        // Se o grafo for não direcionado, descomente a linha abaixo
        matrizadjacencias[verticedestino][verticeorigem] = 0;
    }
    }
}


bool Grafo::ehConexo() 
{
    if (numvertices == 0) {
        return true; // Um grafo vazio pode ser considerado conexo
    }

    // Inicializa o vetor de visitados
    bool *visitados = new bool[numvertices];
    for (int i = 0; i < numvertices; i++) {
        visitados[i] = false;
    }

    // Fila para a BFS
    filadinamica fila;

    // Começa a BFS a partir do primeiro vértice
    fila.inserir(to_string(0));
    visitados[0] = true;
    int visitadosCount = 1;

    while (!fila.estavazio()) {
        int verticeAtual = stoi(fila.primeiroFila());
        fila.remover();

        for (int i = 0; i < numvertices; i++) {
            if (matrizadjacencias[verticeAtual][i] != arestanula and !visitados[i]) {
                fila.inserir(to_string(i));
                visitados[i] = true;
                visitadosCount++;
            }
        }
    }

    delete[] visitados;

    // Se todos os vértices foram visitados, o grafo é conexo
    return visitadosCount == numvertices;
}

void Grafo::possuiciclos()
{
  int contador = 0;
  for (int i = 0; i < numvertices; i++){
      if (matrizadjacencias[i][i] == 1){
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

bool Grafo::ehfortementeconexo() 
{
    // Inicializa um array para marcar os vértices visitados
    bool* visitado = new bool[numvertices];
    for (int i = 0; i < numvertices; ++i) {
        visitado[i] = false;
    }
    
    pilhadinamica pilha;
    
    
    pilha.inserir(vertices[0]);
    while (!pilha.estavazio()) {
        string u_str = pilha.remover();
        int u = stoi(u_str); 
        
        if (!visitado[u]) {
            visitado[u] = true;
            
            // Percorre todos os vértices adjacentes ao vértice atual
            for (int i = 0; i < numvertices; i++) {
                // Se houver uma aresta entre os vértices e o vértice adjacente não foi visitado
                if (matrizadjacencias[u][i] != arestanula and !visitado[i]) {
                    // Insere o vértice adjacente na pilha para visita posterior
                    pilha.inserir(to_string(i)); 
                }
            }
        }
    }
    
    bool fortementeConexo = true;
    for (int i = 0; i < numvertices; ++i) {
        // Se algum vértice não foi visitado, o grafo não é fortemente conexo
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


void Grafo::verificaExistenciaAresta(string saida, string entrada)
{
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
}

void Grafo::prim() 
{
    if (ehConexo()){
  
    vector<int> pais(numvertices, -1);
    vector<int> distancia(numvertices, INT_MAX); // Distância em número de arestas
    vector<bool> naArvore(numvertices, false);

    auto executarPrim = [&](int inicio) {
        distancia[inicio] = 0;
        pais[inicio] = -1;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> filaPrioridade;

        filaPrioridade.push(make_pair(distancia[inicio], inicio));

        // Loop principal do algoritmo de Prim
        while (!filaPrioridade.empty()) {
            int u = filaPrioridade.top().second;
            filaPrioridade.pop();

            if (naArvore[u]) continue; // Se já está na árvore, pular

            naArvore[u] = true;

            for (int v = 0; v < numvertices; ++v) {
                if (matrizadjacencias[u][v] && !naArvore[v] && distancia[v] > distancia[u] + 1) {
                    pais[v] = u;
                    distancia[v] = distancia[u] + 1;
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
            cout << "(" << vertices[pais[i]] << ", " << vertices[i] << ") - Distância: " << distancia[i] - distancia[pais[i]] << "\n";
        }
    }
    } else {
      cerr << "Grafo não é conexo, logo não é possivel executar prim." << endl;
    }
}