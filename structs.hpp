#include <iostream>
#include <fstream>

#ifndef STRUCTS_HPP
#define STRUCTS_HPP
using namespace std;
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
struct arquivo {
    fstream *f;
    int pos, MAX, *buffer;
};;
#endif