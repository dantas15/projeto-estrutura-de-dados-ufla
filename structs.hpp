#include <iostream>
#include <fstream>

#ifndef STRUCTS_HPP
#define STRUCTS_HPP
using namespace std;

struct Atleta {
    int id;
    char nome[255];
    char sex[25];
    int idade;
    float altura;
    float peso;
    char time[255];
    int valido = 1;
    int posicao = -1;

    Atleta() {
      id = -1;
      memset(nome, 0, sizeof(nome));
      memset(sex, 0, sizeof(sex));
      idade = 0;
      altura = 0;
      peso = 0;
      memset(time, 0, sizeof(time));
      valido = true;
      posicao = -1;
    }

    int particao(int menor, int maior) {
      Atleta pivo = this[maior];

      int i = (menor - 1);

      for (int j = menor; j <= maior; j++) {

        // Se o primeiro nome é menor que o segundo nome, trocam
        if ((strcasecmp(this[j].nome, pivo.nome)) < 0) {

          i++;
          swap(this[i], this[j]);

          // Se os dois nomes são iguais, usa id como desempate
        } else if (((strcasecmp(this[j].nome, pivo.nome)) == 0) && (this[j].id < pivo.id)) {
          i++;
          swap(this[i], this[j]);
        }
      }

      swap(this[i + 1], this[maior]);

      return (i + 1);
    }
    void qsort(int menor, int maior) {
      if (menor < maior) {
        int pi = particao(menor, maior);
        qsort(menor, pi - 1);
        qsort(pi + 1, maior);
      }
    }
};
#endif