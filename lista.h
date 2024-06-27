#ifndef LISTA_H
#define LISTA_H
#include <string>
#include "noh.h"

class Lista {
public:
    struct No {
        int vertice;
        int peso;
        No* proximo;
    };

private:
    No* proximo;

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
    No* fim() const;
    string obter(int posicao);

};

#endif // LISTA_H
