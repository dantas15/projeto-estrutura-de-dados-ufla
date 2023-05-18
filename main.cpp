#include <iostream>

using namespace std;

const string NOME_ARQ_BINARIO = "atletas.bin";

struct Atleta {
  int id;
  char nome[255];
  int idade;
  float altura;
  float peso;
  char time[255];
};

int main()
{
  cout << "Ola, mundo!";

  return EXIT_SUCCESS;
}