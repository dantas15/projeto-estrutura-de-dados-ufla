#include <cstring>
#include <string>
#include <cstdio>

#include "structs.hpp"

#ifndef PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
#define PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
const int MAXIMO_CHUNK = 1000;
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

void preencheBuffer(arquivo* arq, int K) {
  if (arq->f != NULL) {
    arq->pos = 0;
    arq->MAX = 0;
    for (int i = 0; i < K || arq->f->eof(); i++) {
      if (!arq->f->eof()) {
        arq->f->open(arq->nome, ios::in);
        arq->f->read((char *) &arq->buffer[arq->MAX], sizeof(Atleta));
        arq->MAX++;
      }
    }
    arq->f->close();
    arq->f = NULL;
  }
}
int procuraMenorPorId(arquivo *arq, int numArqs, int K, Atleta *menor) {
  int idx = -1;

  // verifica todos, atribui idx para o menor
  for (int i = 0; i < numArqs; i++) {
    if(arq[i].pos < arq[i].MAX) {
      if (idx == -1) {
        idx = 1;
      } else if (arq[i].buffer[arq[i].pos].id < arq[idx].buffer[arq[idx].pos].id) {
        idx = i;
      }
    }
  }

  if (idx != -1) {
    *menor = arq[idx].buffer[arq[idx].pos];
    arq[idx].pos++;
    if (arq[idx].pos == arq[idx].MAX) {
      // achou um menor
      // atualiza a posição do buffer ou enche se estiver vazio
      preencheBuffer(&arq[idx], K);
    }
    return 1;
  } else {
    return 0;
  }
}
void salvaArquivo(
    string nomeArquivo,
    Atleta *arr,
    int tam
    /*, caso tenho que modificar a linha original: int mudaLinhaFinal*/
    ) {
  ofstream outputF(nomeArquivo, ios::app);
  if(!outputF.is_open()) {
    // early return
    return;
  }

  // escrever o arquivo com a struct (mesma coisa do ExportarParaCSV)
  outputF << arr[tam - 1].id << ";" << arr[tam - 1].nome << ";" << arr[tam - 1].sex << ";" << arr[tam - 1].idade << ";" << arr[tam - 1].altura << ";" << arr[tam - 1].peso << ";" << arr[tam - 1].time << endl;
  outputF.close();
}
void merge(string nomeArquivo, int numArqs, int qntChunk) {
  string novoArq;
  Atleta *buffer = new Atleta[qntChunk];

  arquivo *arq = new arquivo[numArqs];

  // cria os arquivos temporários
  for (int i = 0; i < numArqs; i++) {
    novoArq = "temp_" + to_string(i + 1) + ".txt";
    // dados do arquivo
    arq[i].f = new fstream();
    arq[i].nome = novoArq;
    arq[i].f->open(arq[i].nome, fstream::in); // abre o arquivo em modo leitura
    arq[i].MAX = 0;
    arq[i].pos = 0;
    // aloca o buffer com a quantidade de elementos que é
    // possível carregar na memória para cada um dos buffers
    arq[i].buffer = new Atleta[qntChunk];
    preencheBuffer(&arq[i], qntChunk);
  }

  int qntBuffer = 0;
  Atleta menor;
  while (procuraMenorPorId(arq, numArqs, qntChunk, &menor) == 1) {
    buffer[qntBuffer] = menor;
    qntBuffer++;

    // buffer está cheio
    if(qntBuffer == qntChunk) {
      salvaArquivo(nomeArquivo, buffer, qntChunk);
      qntBuffer = 0;
    }
  }

  // salva arquivos no buffer
  if (qntBuffer != 0) {
    salvaArquivo(nomeArquivo, buffer, qntBuffer);
  }

  for(int i = 0; i < numArqs; i++) {
    delete [] arq[i].buffer;
  }
  delete [] arq;
  delete [] buffer;
}

int criaArquivoOrdenadosPorId(string nomeArquivo) {
  Atleta *arr = new Atleta[MAXIMO_CHUNK];
  int cont = 0, total = 0;

  ifstream lerArq(nomeArquivo);
  string novoNomeArq;
  while (!lerArq.eof()) {
    lerArq.read((char *) &arr[total], sizeof(Atleta));
    total++;
    if (total == MAXIMO_CHUNK) {
      cont++;
      novoNomeArq = "temp_" + to_string(cont) + ".txt";
      qSortPorId(arr, 0,total - 1);
      salvaArquivo(novoNomeArq, arr, total);
    }
  }
  if (total > 0) {
    cont++;
    novoNomeArq = "temp_" + to_string(cont) + ".txt";
    qSortPorId(arr, 0, total);
    salvaArquivo(novoNomeArq, arr, total);
  }
  lerArq.close();
  return cont;
}

void mergeSortExternoPorId(string nomeArquivo) {
  string novoNomeArq;
  int numArqs = criaArquivoOrdenadosPorId(nomeArquivo);
  int i, k = MAXIMO_CHUNK / (numArqs + 1);

  remove(nomeArquivo.c_str());
  merge(nomeArquivo, numArqs, k);

  for(i = 0; i < numArqs; i++) {
    novoNomeArq = "temp_" + to_string(i + 1) + ".txt";
    remove(novoNomeArq.c_str());
  }
}

#endif //PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
