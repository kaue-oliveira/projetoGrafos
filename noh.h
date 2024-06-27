#ifndef NO_H 
#define NO_H 

#include <iostream>
using namespace std;

typedef string TipoItem; //mudar para TipoItem para facilitar caso mude


struct No
{
  string valor;
  No* proximo;
};

#endif