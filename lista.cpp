#include "lista.h"
#include <iostream>

Lista::Lista() : head(nullptr) {}

Lista::~Lista() {
    limpar();
}

void Lista::inserir(int vertice, int peso) {
    No* novo = new No(vertice, peso);
    novo->proximo = head;
    head = novo;
}

void Lista::remover(int vertice) {
    No* atual = head;
    No* anterior = nullptr;

    while (atual != nullptr && atual->vertice != vertice) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual != nullptr) {
        if (anterior == nullptr) {
            head = atual->proximo;
        } else {
            anterior->proximo = atual->proximo;
        }
        delete atual;
    }
}

int Lista::contar(int vertice) const {
    No* atual = head;
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
    No* atual = head;
    int count = 0;

    while (atual != nullptr) {
        count++;
        atual = atual->proximo;
    }

    return count;
}

void Lista::limpar() {
    while (head != nullptr) {
        No* temp = head;
        head = head->proximo;
        delete temp;
    }
}

void Lista::imprimir(const std::string* vertices) const {
    No* atual = head;
    while (atual != nullptr) {
        std::cout << " -> " << vertices[atual->vertice];
        atual = atual->proximo;
    }
}

Lista::No* Lista::inicio() const {
    return head;
}
