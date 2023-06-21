#include <iostream>

#ifndef ATLETA_HPP
#define ATLETA_HPP
struct Atleta
{
  int id;
  char nome[255];
  char sex[25];
  int idade;
  float altura;
  float peso;
  char time[255];
  int valido = 1;
};
#endif