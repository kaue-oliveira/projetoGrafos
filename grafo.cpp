// Arquivo grafo.cpp
#include "grafo.h"
const string Nomearquivo = "grafo.txt";

/*Construtor para o grafo, ele inicia a matriz e tambem ajusta as arestas que existem, ou coisas do tipo*/
Grafo::Grafo(int max) 
{
    numvertices = 0;
    maxvertices = max;
    arestanula = 0;
    direcionado = false; 

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
/*Destrutor para o grafo, ele vai desalocando linhas e colunas do grafo*/
Grafo::~Grafo() 
{
    delete[] vertices;
    for (int i = 0; i < maxvertices; i++) {
        delete[] matrizadjacencias[i];
    }
    delete[] matrizadjacencias;
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
  for (int i = 0; i < maxvertices; i++){
    if (vertices[i] == verticeEditar) {
      existe = true;
    }
  }

  if (existe == false) {
      throw invalid_argument("O vertice não existe =(");
  }
  int vertice;
  vertice = stoi(verticeEditar);
  vertice--;
  for (int i = 0; i < numvertices; i++) {
    for (int j = 0; j < numvertices; j++) {
        matrizadjacencias[i][vertice] = 0; 
    }
  }

  for (int i = 0; i < numvertices; i++) {
    for (int j = 0; j < numvertices; j++) {
        matrizadjacencias[vertice][j] = 0; 
    }
  }
  
  string verticesTemp[numvertices];
  for (int i = 0; i < numvertices; i++) {
    if (verticeEditar != vertices[i]) {
    int j = 0;
    verticesTemp[i] = vertices[i];
    j++;
    }
  }
  numvertices--;
  
  for (int i = 0; i < numvertices; i++){
  vertices[i] = verticesTemp[i];
  }

  cout << "Vertice removido com sucesso =)" << endl;
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

    // Verificando se os parênteses estão presentes nas arestas
    string arestas_str = line.substr(arestas_pos);
    if (arestas_str.find("(") == string::npos or arestas_str.find(")") == string::npos) {
        cerr << "Erro de formatação: falta de parênteses nas arestas." << endl
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
/*
void Grafo::reescreverArquivo()
{
  string nomeArquivo;
  ofstream (nomeArquivo);

  

}
*/