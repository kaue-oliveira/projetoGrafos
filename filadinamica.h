//Fila Dinâmica = Dynamic Queue

#ifndef FILADINAMICA_H 
#define FILADINAMICA_H 
#include <string>
#include "noh.h"

class filadinamica{ 

    private:
    No* primeiro; //front
    No* ultimo; //rear

    public:
    filadinamica(); // Constructor
    ~filadinamica(); // Destrutor  
    bool estavazio(); //isEmpty
    bool estacheio(); //isFull
    void inserir(string item); //enqueue //push
    string remover(); //dequeue //pop
    void imprimir(); //print
    string primeiroFila();
  
};

#endif