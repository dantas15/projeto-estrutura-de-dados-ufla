#include <cstring>
#include <string>
#include <cstdio>
#include <fstream>

#include "structs.hpp"

#ifndef PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
#define PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
const int MAXIMO_CHUNK = 1000;

/*
 ** Funções para ordenação interna **
*/
void mudaPos(Atleta& a, Atleta& b) {
  Atleta temp = a;
  a = b;
  b = temp;
}
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
        mudaPos(arr[i], arr[j]);
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

void preencheBuffer(arquivo* arq, int tamBuffer) {
  fstream arqBin;
  arqBin.open(arq->nome, ios::binary | ios::in);

  if (arqBin.good() && arq->valido) {
    arq->pos = 0;
    arq->MAX = 0;
    for (int i = 0; i < tamBuffer || arqBin.eof(); i++) {
      arqBin.read((char *) &arq->buffer[arq->MAX], sizeof(Atleta));
      arq->MAX++;
    }
    arq->valido = false;
    arqBin.close();
  }
}
void salvaArquivo(
    string nomeArquivo,
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

/* Ordenação por id */
int procuraMenorPorId(arquivo *arq, int numArqs, int K, Atleta *menor) {
  int idxMenorId = -1;

  // verifica o menor elemento no buffer de todos os arquivos
  for (int i = 0; i < numArqs; i++) {
    // verifica se ainda nao atingiu o limite do aquivo
    if(arq[i].pos < arq[i].MAX) {
      // se estiver na primeira iteracao, salva como menor
      if (idxMenorId == -1) {
        idxMenorId = i;
      // senao, verifica se o id atual é menor que o idxMenorId
      } else if (arq[i].buffer[arq[i].pos].id < arq[idxMenorId].buffer[arq[idxMenorId].pos].id) {
        idxMenorId = i;
      }
    }
  }

  // se encontrou o menor dos arquivos
  if (idxMenorId != -1) {
    *menor = arq[idxMenorId].buffer[arq[idxMenorId].pos];
    // aqui 'pos' é incrementado para, na próxima iteração,
    // o segundo do arquivo ser comparado
    arq[idxMenorId].pos++;

    // agora verificamos se já percorremos
    // todos os elementos do arquivo
    if (arq[idxMenorId].pos == arq[idxMenorId].MAX) {
      // se sim, preenchemos o buffer
      preencheBuffer(&arq[idxMenorId], K);
    }
    return 1; // codigo que encontramos menor
  } else {
    return 0; // codigo que nao encontramos menor
  }
}
void mergePorId(string nomeArquivo, int numArqs, int qntChunk) {
  string novoNomeArquivo;
  Atleta *buffer = new Atleta[qntChunk];

  arquivo *arq = new arquivo[numArqs];

  // cria os arquivos temporários
  for (int i = 0; i < numArqs; i++) {
    novoNomeArquivo = "temp_" + to_string(i + 1) + ".bin";
    // dados do arquivo
    arq[i].nome = novoNomeArquivo;
    arq[i].MAX = 0;
    arq[i].pos = 0;
    // aloca o buffer com a quantidade de elementos que é
    // possível carregar na memória para cada um dos buffers
    arq[i].buffer = new Atleta[qntChunk];
    preencheBuffer(&arq[i], qntChunk);
  }

  int qntBuffer = 0;
  Atleta menorAtleta;
  while (procuraMenorPorId(arq, numArqs, qntChunk, &menorAtleta) == 1) {
    buffer[qntBuffer] = menorAtleta;
    qntBuffer++;
    // buffer está cheio
    if(qntBuffer == qntChunk) {
      salvaArquivo(nomeArquivo, buffer, qntChunk);
      qntBuffer = 0;
    }
  }

  // caso tenha arquivos nao salvos no buffer ainda
  // (dados nao multiplos)
  if (qntBuffer != 0) {
    salvaArquivo(nomeArquivo, buffer, qntBuffer);
  }

  // desalocando memória depois do merge dos arquivos
  for(int i = 0; i < numArqs; i++) {
    delete [] arq[i].buffer;
  }
  delete [] arq;
  delete [] buffer;
}

int criaArquivoOrdenadosPorId(string nomeArquivo) {
  Atleta *arr = new Atleta[MAXIMO_CHUNK];
  int cont = 0, total = 0;

  // abrimos o arquivo no final para pegarmos o qntAtletas inicialmente
  fstream lerArq(nomeArquivo, ios::binary | ios::in | ios::ate);
  Atleta atletaAux;
  string novoNomeArq;

  int qntAtletas = lerArq.tellg() / sizeof(Atleta);
  for (int i = 0; i < qntAtletas; i++) {
    // ler e salvar atletas
    lerArq.seekg(i * sizeof(Atleta), ios::beg); // apontar
    lerArq.read((char *)&atletaAux, sizeof(Atleta));
    arr[total] = atletaAux;
    total++;
    if (total == MAXIMO_CHUNK) {
      cont++;
      novoNomeArq = "temp_" + to_string(cont) + ".bin";
      qSortPorId(arr, 0,total - 1);
      salvaArquivo(novoNomeArq, arr, total);
      total = 0;
    }
  }
  if (total > 0) {
    cont++;
    novoNomeArq = "temp_" + to_string(cont) + ".bin";
    qSortPorId(arr, 0, total - 1);
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
  mergePorId(nomeArquivo, numArqs, k);

  for(i = 0; i < numArqs; i++) {
    novoNomeArq = "temp_" + to_string(i + 1) + ".bin";
    remove(novoNomeArq.c_str());
  }

  cout << "\nArquivo ordenado por Id com sucesso!\n";
}

/* Ordenação por nome */
int procuraMenorPorNome(arquivo *arq, int numArqs, int K, Atleta *menor) {
  int idxMenorNome = -1;

  // verifica o menor elemento no buffer de todos os arquivos
  for (int i = 0; i < numArqs; i++) {
    // verifica se ainda nao atingiu o limite do aquivo
    if(arq[i].pos < arq[i].MAX) {
      // se estiver na primeira iteracao, salva como menor
      if (idxMenorNome == -1) {
        idxMenorNome = i;
        // senao, verifica se o nome atual é menor que o idxMenorNome
      } else if (comparaNomes(arq[i].buffer[arq[i].pos].nome, arq[idxMenorNome].buffer[arq[idxMenorNome].pos].nome) < 0)
        idxMenorNome = i;
      }
    }

  // se encontrou o menor dos arquivos
  if (idxMenorNome != -1) {
    *menor = arq[idxMenorNome].buffer[arq[idxMenorNome].pos];
    // aqui 'pos' é incrementado para, na próxima iteração,
    // o segundo do arquivo ser comparado
    arq[idxMenorNome].pos++;

    // agora verificamos se já percorremos
    // todos os elementos do arquivo
    if (arq[idxMenorNome].pos == arq[idxMenorNome].MAX) {
      // se sim, preenchemos o buffer
      preencheBuffer(&arq[idxMenorNome], K);
    }
    return 1; // codigo que encontramos menor
  } else {
    return 0; // codigo que nao encontramos menor
  }
}
void mergePorNome(string nomeArquivo, int numArqs, int qntChunk) {
  string novoNomeArquivo;
  Atleta *buffer = new Atleta[qntChunk];

  arquivo *arq = new arquivo[numArqs];

  // cria os arquivos temporários
  for (int i = 0; i < numArqs; i++) {
    novoNomeArquivo = "temp_" + to_string(i + 1) + ".bin";
    // dados do arquivo
    arq[i].nome = novoNomeArquivo;
    arq[i].MAX = 0;
    arq[i].pos = 0;
    // aloca o buffer com a quantidade de elementos que é
    // possível carregar na memória para cada um dos buffers
    arq[i].buffer = new Atleta[qntChunk];
    preencheBuffer(&arq[i], qntChunk);
  }

  int qntBuffer = 0;
  Atleta menorAtleta;
  while (procuraMenorPorNome(arq, numArqs, qntChunk, &menorAtleta) == 1) {
    buffer[qntBuffer] = menorAtleta;
    qntBuffer++;
    // buffer está cheio
    if(qntBuffer == qntChunk) {
      salvaArquivo(nomeArquivo, buffer, qntChunk);
      qntBuffer = 0;
    }
  }

  // caso tenha arquivos nao salvos no buffer ainda
  // (dados nao multiplos)
  if (qntBuffer != 0) {
    salvaArquivo(nomeArquivo, buffer, qntBuffer);
  }

  // desalocando memória depois do merge dos arquivos
  for(int i = 0; i < numArqs; i++) {
    delete [] arq[i].buffer;
  }
  delete [] arq;
  delete [] buffer;
}

int criaArquivoOrdenadosPorNome(string nomeArquivo) {
  Atleta *arr = new Atleta[MAXIMO_CHUNK];
  int cont = 0, total = 0;

  // abrimos o arquivo no final para pegarmos o qntAtletas inicialmente
  fstream lerArq(nomeArquivo, ios::binary | ios::in | ios::ate);
  Atleta atletaAux;
  string novoNomeArq;
  int qntAtletas = lerArq.tellg() / sizeof(Atleta);

  for (int i = 0; i < qntAtletas; i++) {
    // ler e salvar atletas
    lerArq.seekg(i * sizeof(Atleta), ios::beg); // apontar
    lerArq.read((char *)&atletaAux, sizeof(Atleta));
    arr[total] = atletaAux;
    total++;
    if (total == MAXIMO_CHUNK) {
      cont++;
      novoNomeArq = "temp_" + to_string(cont) + ".bin";
      qSortPorNome(arr, 0,total - 1);
      salvaArquivo(novoNomeArq, arr, total);
      total = 0;
    }
  }
  if (total > 0) {
    cont++;
    novoNomeArq = "temp_" + to_string(cont) + ".bin";
    qSortPorId(arr, 0, total - 1);
    salvaArquivo(novoNomeArq, arr, total);
  }
  lerArq.close();
  return cont;
}

void mergeSortExternoPorNome(string nomeArquivo) {
  string novoNomeArq;
  int numArqs = criaArquivoOrdenadosPorNome(nomeArquivo);
  int i, k = MAXIMO_CHUNK / (numArqs + 1);

  remove(nomeArquivo.c_str());
  mergePorNome(nomeArquivo, numArqs, k);

  for(i = 0; i < numArqs; i++) {
    novoNomeArq = "temp_" + to_string(i + 1) + ".bin";
    remove(novoNomeArq.c_str());
  }

  cout << "\nArquivo ordenado por nome com sucesso!\n";
}
#endif //PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
