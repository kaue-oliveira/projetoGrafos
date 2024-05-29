// Pilha Dinâmica = Dynamic Stack

#ifndef PILHADINAMICA_H 
#define PILHADINAMICA_H 

#include "noh.h"

class pilhadinamica{
    private:
    No* NoTopo;

    public:
    pilhadinamica(); //construtor //dynamic stack
    ~pilhadinamica(); //destrutor
    bool estavazio(); //isempty
    bool estacheio(); //tem memória //isfull
    void inserir(string item); //push
    string remover(); //pop
    void imprimir(); //print
    string topo();

    
};

#endif