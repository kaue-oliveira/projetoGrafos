#include "lista.h"
#include <iostream>

Lista::Lista(){
  proximo == nullptr;
}
Lista::~Lista() {
    limpar();
}

void Lista::inserir(int verticeNew, int pesoNew) {
    No* novo = new No();
    novo->peso = pesoNew;
    novo->vertice = verticeNew;
    novo->proximo = proximo;
    proximo = novo;
}

void Lista::remover(int vertice) {
    No* atual = proximo;
    No* anterior = nullptr;

    while (atual != nullptr && atual->vertice != vertice) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual != nullptr) {
        if (anterior == nullptr) {
            proximo = atual->proximo;
        } else {
            anterior->proximo = atual->proximo;
        }
        delete atual;
    }
}

int Lista::contar(int vertice) const {
    No* atual = proximo;
    int count = 0;

    while (atual != nullptr) {
        if (atual->vertice == vertice) {
            count++;
        }
        atual = atual->proximo;
    }

    return count;
}

int Lista::tamanho() const {
    No* atual = proximo;
    int count = 0;

    while (atual != nullptr) {
        count++;
        atual = atual->proximo;
    }

    return count;
}

void Lista::limpar() {
    while (proximo != nullptr) {
        No* temp = proximo;
        proximo = proximo->proximo;
        delete temp;
    }
}

void Lista::imprimir(const std::string* vertices) const {
    No* atual = proximo;
    while (atual != nullptr) {
        std::cout << " -> " << vertices[atual->vertice];
        atual = atual->proximo;
    }
}

Lista::No* Lista::inicio() const {
    return proximo;
}


Lista::No* Lista::fim() const {
    if (proximo == nullptr) {
        return nullptr;
    }

    No* atual = proximo;
    while (atual->proximo != nullptr) {
        atual = atual->proximo;
    }
    return atual;
}
std::string Lista::obter(int posicao) {
    No* atual = proximo;
    int i = 0;
    while (atual != nullptr && i < posicao) {
        atual = atual->proximo;
        i++;
    }
    if (atual != nullptr) {
        return std::to_string(atual->vertice);
    } else {
        return ""; // Retorna uma string vazia se a posição for inválida
    }
}


