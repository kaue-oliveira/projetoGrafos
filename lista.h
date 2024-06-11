#ifndef LISTA_H
#define LISTA_H
#include <string>
class Lista {
public:
    struct No {
        int vertice;
        int peso;
        No* proximo;
        No(int v, int p) : vertice(v), peso(p), proximo(nullptr) {}
    };

private:
    No* head;

public:
    Lista();
    ~Lista();

    void inserir(int vertice, int peso);
    void remover(int vertice);
    int contar(int vertice) const;
    int tamanho() const;
    void limpar();
    void imprimir(const std::string* vertices) const;
    No* inicio() const;
};

#endif // LISTA_H
