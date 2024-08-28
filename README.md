# Graph Work (UFLA)

## Kauê de Oliveira; Thiago Azevedo

# `Graph` Class

The `Graph` class provides a representation and operations for directed and undirected graphs, weighted and unweighted. The following describes the main functions and attributes of the class.

## Attributes

- `numvertices`: Current number of vertices in the graph.
- `maxvertices`: Maximum number of vertices the graph can have.
- `arestanula`: Value to indicate null edges.
- `valorado`: Indicates if the graph is weighted.
- `direcionado`: Indicates if the graph is directed.
- `vertices`: Pointer to an array of vertices.
- `marcador`: Pointer to an array of boolean markers.
- `listaAdjacencia`: Pointer to the adjacency list of the graph.

## Constructors and Destructors

- `Grafo(int max, int nulo)`: Constructor that initializes the graph with a maximum number of vertices and a value for null edges.
- `~Grafo()`: Destructor that releases allocated resources.

## Graph Manipulation

- `void defineDirecionado(std::string resposta)`: Defines whether the graph is directed or not based on the provided response.
- `void inserearesta(std::string Nosaida, std::string Noentrada)`: Inserts an edge between the outgoing and incoming vertices.
- `void inserearestaValorado(std::string Nosaida, std::string Noentrada, std::string peso)`: Inserts a weighted edge between the outgoing and incoming vertices.
- `void inserevertice(std::string item)`: Inserts a new vertex into the graph.
- `void removervertice(std::string verticeEditar)`: Removes a vertex from the graph.
- `void removeraresta(std::string origem, std::string destino)`: Removes an edge between two vertices.

## File Handling

- `void lergrafo()`: Reads the graph from a file.
- `void reescreverArquivo()`: Rewrites the file with the current graph information.
- `bool verifica(const std::string& line)`: Checks the validity of a line from the file.

## Graph Operations

- `int obterindice(std::string item)`: Obtains the index of a vertex.
- `void obtergrau(std::string item)`: Obtains the degree of a vertex.
- `int obtergrauaux(std::string item)`: Obtains the auxiliary degree of a vertex.
- `bool ehConexo()`: Checks if the graph is connected.
- `bool ehdirecionado()`: Checks if the graph is directed.
- `bool ehvalorado()`: Checks if the graph is weighted.
- `int qtdvertice()`: Obtains the number of vertices.
- `int qtdarestas()`: Obtains the number of edges.
- `void verificaExistencia(std::string vertice)`: Checks the existence of a vertex.
- `void verificaExistenciaAresta(std::string saida, std::string entrada, std::string peso)`: Checks the existence of an edge.
- `int obtergrauEntrada(std::string item)`: Obtains the in-degree of a vertex.
- `int obtergrauSaida(std::string item)`: Obtains the out-degree of a vertex.
- `void limpamarcador()`: Clears the vertex markers.

## Algorithms and Advanced Functions

- `void imprimirvertices()`: Prints the vertices of the graph.
- `void possuiCiclos()`: Checks if the graph contains cycles.
- `void eheuleriano()`: Checks if the graph is Eulerian.
- `bool ehfortementeconexo()`: Checks if the graph is strongly connected.
- `bool ehCiclicoUtil(int v, std::vector<bool>& visitado, std::vector<bool>& pilha)`: Helper function to check cycles.
- `bool eheulerianoAux()`: Helper function to check if the graph is Eulerian.
- `void ehCiclico()`: Checks if the graph is cyclic.
- `void ehPlanar()`: Checks if the graph is planar.
- `void buscaemlarguraArvore()`: Performs a breadth-first search and creates a tree.
- `void buscaemprofundidadeArvore()`: Performs a depth-first search and creates a tree.
- `void executarBuscaEmLargura(int inicio, vector<int>& pais, vector<int>& distancia, vector<bool>& visitado)`: Executes breadth-first search.
- `void executarBuscaEmProfundidade(int inicio, vector<int>& pais, vector<int>& distancia, vector<bool>& visitado)`: Executes depth-first search.
- `void buscaemlargura(std::string origem, std::string destino)`: Performs breadth-first search between two vertices.
- `void buscaemprofundidade(std::string origem, std::string destino)`: Performs depth-first search between two vertices.
- `void prim()`: Executes Prim's algorithm to find the minimum spanning tree.
- `void tarjanArticulacao(int u, std::vector<int>& disc, std::vector<int>& low, std::vector<int>& pai, std::vector<bool>& articulacao, int& tempo)`: Finds articulation points using Tarjan's algorithm.
- `void encontrarVerticesArticulacao()`: Finds all articulation points in the graph.
- `void listarCaminhoHamiltoniano()`: Lists the Hamiltonian path.
- `bool hamiltonianUtil(int pos, std::vector<int>& caminho, std::vector<bool>& visitado)`: Helper function to find a Hamiltonian path.
- `bool ehHamiltoniano()`: Checks if the graph is Hamiltonian.
- `void exibirCaminhoEuleriano()`: Displays the Eulerian path in the graph.
- `void exibirCaminhoMinimoDijkstra(string origem, string destino)`: Displays the shortest path between two vertices using Dijkstra's algorithm.
- `void exibirOrdemTopologica()`: Displays the topological order of vertices.
- `void exibirComponentesConexas()`: Displays the connected components of the graph.
- `void exibirArestasPonte()`: Displays the bridge edges in the graph.
- `void tarjanPontesUtil(int u, std::vector<int>& disc, std::vector<int>& low, std::vector<int>& pai, std::vector<bool>& pontes, int& tempo, std::vector<std::pair<int, int>>& listaPontes)`: Finds bridge edges using Tarjan's algorithm.
- `void ehBipartido()`: Checks if the graph is bipartite.

## Functions to Implement or Adjust

- `int fluxoMaximo(std::string origem, std::string destino)`: Calculates the maximum flow between two vertices.
- `int bfs(int s, int t, std::vector<int>& pai)`: Performs breadth-first search to find the path between two vertices.
- `void dfs(int inicio, vector<bool>& visitados)`: Performs depth-first search.

## Future Features

- `void gerarFechamentoTransitivo()`: Generates the transitive closure of the graph.
- `void listarArestas()`: Lists all edges in the graph.

# The main function is still under development
