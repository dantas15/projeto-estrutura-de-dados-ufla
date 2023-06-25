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
  Atleta() {
    id = -1;
    memset(nome, 0, sizeof(nome));
    memset(sex, 0, sizeof(sex));
    idade = 0;
    altura = 0;
    peso = 0;
    memset(time, 0, sizeof(time));
    valido = true;
  }
};
struct arquivo {
    string nome;
    bool valido = true;
    int pos, MAX;
    Atleta *buffer;
};
#endif