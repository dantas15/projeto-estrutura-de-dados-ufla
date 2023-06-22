#include "cstring"
#include "atleta.hpp"

#ifndef PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
#define PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
void mudaPos(Atleta& a, Atleta& b) {
  Atleta temp = a;
  a = b;
  b = temp;
}
void qSortPorId(Atleta arr[], int low, int high) {
  if (low < high) {
    int pivot = arr[high].id;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
      if (arr[j].id < pivot) {
        i++;
        mudaPos(arr[i], arr[j]);
      }
    }

    mudaPos(arr[i + 1], arr[high]);
    qSortPorId(arr, low, i);
    qSortPorId(arr, i + 2, high);
  }
}
int comparaNomes(const char* nome1, const char* nome2) {
  return strcmp(nome1, nome2);
}
void qSortPorNome(Atleta arr[], int low, int high) {
  if (low < high) {
    int i = low;
    int j = high;
    const char* pivot = arr[(low + high) / 2].nome;

    while (i <= j) {
      while (comparaNomes(arr[i].nome, pivot) < 0)
        i++;
      while (comparaNomes(arr[j].nome, pivot) > 0)
        j--;

      if (i <= j) {
        mudaPos(arr[i], arr[j]);
        i++;
        j--;
      }
    }

    qSortPorNome(arr, low, j);
    qSortPorNome(arr, i, high);
  }
}

#endif //PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
