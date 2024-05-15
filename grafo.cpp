// Arquivo grafo.cpp
#include "grafo.h"
#include "filadinamica.h"
#include "pilhadinamica.h"
const string Nomearquivo = "grafo.txt";

/*Construtor para o grafo, ele inicia a matriz e tambem ajusta as arestas que existem, ou coisas do tipo*/
Grafo::Grafo(int max) //construtor
    {
        numvertices = 0;
        maxvertices = max;
        arestanula = 0;

        marcador = new bool[maxvertices]; //novo

        vertices = new TipoItem[maxvertices];

        matrizadjacencias = new int*[maxvertices];
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
    Grafo::~Grafo() //destrutor
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
    return -1; // Retorna -1 se o vértice não for encontrado
}
/*Insere um vertice no grafo(util pra construir ou editar)*/
void Grafo::inserevertice(string item) 
{
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

/*Essa função pode ser bem util futuramente*/
int Grafo::obtergrau(string item) 
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
        throw invalid_argument("O vértice não existe =(");
    }

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

int Grafo::qtdvertice()
{
  int num = numvertices;
  return(num);
}

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
        for (int i = 0; i < maxvertices; i++) {
            marcador[i] = false;
        }
    } else {
        cerr << "Erro: Ponteiro marcador não inicializado." << endl;
    }
}


void Grafo::buscaemlargura(string origem, string destino)
{
    filadinamica filavertices;
    bool encontrado = false;
    limpamarcador();

    int indiceOrigem = obterindice(origem);
    filavertices.inserir(origem);
    marcador[indiceOrigem] = true; // Marcar a origem como visitada

    while (!filavertices.estavazio() && !encontrado) {
        string verticeatual = filavertices.remover();
        cout << "Visitando: " << verticeatual << endl;

        if (verticeatual == destino) {
            cout << "Caminho encontrado!\n";
            encontrado = true;
        } else {
            int indice = obterindice(verticeatual);
            for (int i = 0; i < maxvertices; i++) {
                if (matrizadjacencias[indice][i] != arestanula && !marcador[i]) {
                    cout << "Enfileirando: " << vertices[i] << endl;
                    filavertices.inserir(vertices[i]);
                    marcador[i] = true; // Marcar o vértice como visitado
                }
            }
        }
    }

    if (!encontrado) {
        cout << "Caminho nao encontrado!\n";
    }
}



    


void Grafo::buscaemprofundidade(string origem, string destino)
{
    pilhadinamica pilhavertices;
    bool encontrado = false;
    limpamarcador();

    int indiceOrigem = obterindice(origem);
    pilhavertices.inserir(origem);
    marcador[indiceOrigem] = true; // Marcar a origem como visitada

    while (!pilhavertices.estavazio() && !encontrado) {
        string verticeatual = pilhavertices.remover();
        cout << "Visitando: " << verticeatual << endl;

        if (verticeatual == destino) {
            cout << "Caminho encontrado!\n";
            encontrado = true;
        } else {
            int indice = obterindice(verticeatual);
            for (int i = 0; i < maxvertices; i++) {
                if (matrizadjacencias[indice][i] != arestanula && !marcador[i]) {
                    cout << "Empilhando: " << vertices[i] << endl;
                    pilhavertices.inserir(vertices[i]);
                    marcador[i] = true; // Marcar o vértice como visitado
                }
            }
        }
    }

    if (!encontrado) {
        cout << "Caminho nao encontrado!\n";
    }
}
