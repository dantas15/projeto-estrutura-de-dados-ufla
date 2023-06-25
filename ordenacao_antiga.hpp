#include <cstring>
#include <string>
#include <cstdio>
#include <fstream>

#include "structs.hpp"

#ifndef PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
#define PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP

struct arquivo {
    string nome;
    bool valido = true;
    int pos, MAX;
    Atleta *buffer;
};
const int MAXIMO_POR_ARQUIVO = 1000;

/*
 ** Funções para ordenação interna **
*/
bool comparaMenorAtleta(const Atleta at1, const Atleta at2) {
  int comparaNome = strcmp(at1.nome, at2.nome);

  if (comparaNome < 0) {
    return true;  // at1 é menor que at2
  } else if (comparaNome == 0) {
    return at1.id < at2.id;  // comparando pelo id se at1 é menor que at2
  } else {
    return false;  // at1 é maior que at2
  }
}
void qSort(Atleta *&arr, int low, int high) {
  if (low < high) {
    int i = low;
    int j = high;
    const Atleta pivot = arr[(low + high) / 2];

    while (i <= j) {
      while (comparaMenorAtleta(arr[i], pivot))
        i++;
      while (comparaMenorAtleta(pivot, arr[j]))
        j--;

      if (i <= j) {
        swap(arr[i], arr[j]);
        i++;
        j--;
      }
    }

    qSort(arr, low, j);
    qSort(arr, i, high);
  }
}

/*
 ** Funções para ordenação externa **
*/
string criaArqTemp(int num) {
  return ("temp_" + to_string(num) + ".bin");
}

void preencheBuffer(arquivo* arq, int tamBuffer) {
  fstream arqBin;
  arqBin.open(arq->nome, ios::binary | ios::in | ios::out);

  if (arqBin.good() && arq->valido) {
    arqBin.seekg(0, arqBin.end);
    int tamArquivo = arqBin.tellg() / sizeof(Atleta);
    arq->pos = 0;
    arq->MAX = 0;
    for (int i = 0; i < tamBuffer && i < tamArquivo; i++) {
      arqBin.seekg(i * sizeof(Atleta));
      arqBin.read((char *) &arq->buffer[arq->MAX], sizeof(Atleta));
      arq->MAX++;
    }
    arq->valido = false;
    arqBin.close();
  }
}
void salvaArquivo(
    const string& nomeArquivo,
    Atleta *arr,
    int tam
    /*, caso tenho que modificar a linha original: int mudaLinhaFinal*/
) {
  // pode ser o arquivo temp_{i}.bin
  // então não é possível verificar a existência (vamos criar)
  fstream outputF(nomeArquivo, ios::binary | ios::out | ios::app);

  // já que é arquivo binário,
  // não vamos nos preocupar com quebra de linha no final
  for (int i = 0; i < tam; i++) {
    outputF.write((char *)&arr[i], sizeof(Atleta));
  }

  outputF.close();
}

/* Ordenação por nome e id como desempate */
bool procuraMenor(arquivo *arq, int qntArquivos, int tamBuffer, Atleta *menor) {
  int idx = -1;

  // verifica o menor elemento no buffer de todos os arquivos
  for (int i = 0; i < qntArquivos; i++) {
    // verifica se ainda nao atingiu o limite do aquivo
    if(arq[i].pos < arq[i].MAX) {
      // se estiver na primeira iteracao, salva como menor
      if (idx == -1) {
        idx = i;
        // senao, verifica se o nome atual é menor que o idx
      } else if (comparaMenorAtleta(arq[i].buffer[arq[i].pos], arq[idx].buffer[arq[idx].pos]))
        idx = i;
      }
    }

  // se encontrou o menor dos arquivos
  if (idx != -1) {
    *menor = arq[idx].buffer[arq[idx].pos];
    // aqui 'pos' é incrementado para, na próxima iteração,
    // o segundo do arquivo ser comparado
    arq[idx].pos++;

    // agora verificamos se já percorremos
    // todos os elementos do buffer
    if (arq[idx].pos == arq[idx].MAX) {
      // se sim, preenchemos o buffer
      preencheBuffer(&arq[idx], tamBuffer);
    }
    return true; // sinalizando que encontramos menor
  } else {
    return false; // sinalizando que nao encontramos menor
  }
}
void merge(const string& nomeArquivoOriginal, int qntArquivos, int tamBuffer) {
  Atleta *buffer = new Atleta[tamBuffer];

  arquivo *arquivos = new arquivo[qntArquivos];

  // cria os arquivos temporários
  for (int i = 0; i < qntArquivos; i++) {
    // dados do arquivo
    arquivos[i].nome = criaArqTemp(i + 1);
    arquivos[i].MAX = 0;
    arquivos[i].pos = 0;
    // aloca o buffer com a quantidade de elementos que é
    // possível carregar na memória para cada um dos buffers
    arquivos[i].buffer = new Atleta[tamBuffer];
    preencheBuffer(&arquivos[i], tamBuffer);
  }

  Atleta menorAtleta;
  int qntBuffer = 0;
  while (procuraMenor(arquivos, qntArquivos, tamBuffer, &menorAtleta)) {
    buffer[qntBuffer] = menorAtleta;
    qntBuffer++;
    // buffer está cheio
    if(qntBuffer == tamBuffer) {
      salvaArquivo(nomeArquivoOriginal, buffer, tamBuffer);
      qntBuffer = 0;
    }
  }

  // caso tenha arquivos nao salvos no buffer ainda
  // (dados nao multiplos)
  if (qntBuffer != 0) {
    salvaArquivo(nomeArquivoOriginal, buffer, qntBuffer);
  }

  // desalocando memória depois do merge dos arquivos
  for(int i = 0; i < qntArquivos; i++) {
    delete [] arquivos[i].buffer;
  }
  delete [] arquivos;
  delete [] buffer;
}

int criaArquivoOrdenados(const string& nomeArquivoOriginal) {
  Atleta *arr = new Atleta[MAXIMO_POR_ARQUIVO];
  int qntArquivos = 0, total = 0;

  // abrimos o arquivo no final para pegarmos o qntAtletas inicialmente
  fstream lerArq(nomeArquivoOriginal, ios::binary | ios::in | ios::ate);
  Atleta atletaAux;
  string novoNomeArq;
  int qntAtletas = lerArq.tellg() / sizeof(Atleta);

  for (int i = 0; i < qntAtletas; i++) {
    // ler e salvar atletas
    lerArq.seekg(i * sizeof(Atleta), ios::beg); // apontar
    lerArq.read((char *)&atletaAux, sizeof(Atleta));
    arr[total] = atletaAux;
    total++;
    if (total == MAXIMO_POR_ARQUIVO) {
      qntArquivos++;
      novoNomeArq = criaArqTemp(qntArquivos);
      qSort(arr, 0, total - 1);
      salvaArquivo(novoNomeArq, arr, total);
      total = 0;
    }
  }
  if (total > 0) {
    qntArquivos++;
    novoNomeArq = criaArqTemp(qntArquivos);
    qSort(arr, 0, total - 1);
    salvaArquivo(novoNomeArq, arr, total);
  }
  lerArq.close();
  delete [] arr;
  return qntArquivos;
}

void mergeSortExterno(const string& nomeArquivoOriginal) {
  string novoNomeArq;
  int qntArquivos = criaArquivoOrdenados(nomeArquivoOriginal);
  int tamBuffer = MAXIMO_POR_ARQUIVO / (qntArquivos + 1);

  remove(nomeArquivoOriginal.c_str());
  merge(nomeArquivoOriginal, qntArquivos, tamBuffer);

  for(int i = 0; i < qntArquivos; i++) {
    novoNomeArq = "temp_" + to_string(i + 1) + ".bin";
    remove(novoNomeArq.c_str());
  }

  cout << "\nArquivo ordenado por nome com sucesso!\n";
}
#endif //PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
