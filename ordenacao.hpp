#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <limits>
#include <limits.h>

#include "structs.hpp"

#ifndef PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
#define PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP

int quantidadeAtletas(ifstream &arq) {
  streampos posInicial = arq.tellg();
  arq.seekg(0, arq.end);
  int qntAtletas = arq.tellg() / sizeof(Atleta);
  arq.seekg(posInicial); // voltando pro valor do ponteiro do arquivo inicial
  return qntAtletas;
}

void salvarArquivo(string nome, Atleta arr[], int tam) {
  ofstream arq(nome, ios::binary);
  for (int i = 0; i < tam; i++) {
    arq.write((char *) (&arr[i]), sizeof(Atleta));
  }
  arq.close();
}

class MinHeapNoh {
public:
    Atleta atleta;
    int i;    // posicao dos arquivos temporários
    int j;    // posicao do próximo pacote que será pego do arquivo temporário i
    void alterarNome() {
      atleta.nome[0] = std::numeric_limits<char>::max();
    }

    void alterarPosicao(int &pos) {
      atleta.posicao = pos++;
    };
};

void swap(MinHeapNoh *x, MinHeapNoh *y) {
  MinHeapNoh temp = *x;
  *x = *y;
  *y = temp;
}

class MinHeap {
private:
    MinHeapNoh *dados;
    int tamanho;

    int esquerdo(int i) {
      return 2 * i + 1;
    }

    int direito(int i) {
      return 2 * i + 2;
    }

    void corrigeDescendo(int i) {
      int esq = esquerdo(i);
      int dir = direito(i);

      int menor = i;

      if ((esq < tamanho) && ((strcasecmp(dados[esq].atleta.nome, dados[i].atleta.nome)) < 0)) {
        menor = esq;
      } else if ((esq < tamanho) && ((strcasecmp(dados[esq].atleta.nome, dados[i].atleta.nome)) == 0) &&
                 (dados[esq].atleta.id < dados[i].atleta.id)) {
        menor = esq;
      }

      if ((dir < tamanho) && ((strcasecmp(dados[dir].atleta.nome, dados[menor].atleta.nome)) < 0)) {
        menor = dir;
      } else if ((dir < tamanho) && ((strcasecmp(dados[dir].atleta.nome, dados[menor].atleta.nome)) == 0) &&
                 (dados[dir].atleta.id < dados[menor].atleta.id)) {
        menor = dir;
      }

      if (menor != i) {
        swap(&dados[i], &dados[menor]);
        corrigeDescendo(menor);
      }
    }

public:
    MinHeap(MinHeapNoh arr[], int tam) {
      tamanho = tam;
      dados = arr;
      int i = (tamanho - 1) / 2;
      while (i >= 0) {
        corrigeDescendo(i);
        i--;
      }
    }

    ~MinHeap() {
      delete[] dados;
    };

    MinHeapNoh getMin() {
      return dados[0];
    }

    void inserirMin(MinHeapNoh x) {
      dados[0] = x;
      corrigeDescendo(0);
    }
};

/* Funções de ordenação */
void mergeArquivos(const string& nomeArqInput, const string& nomeArqOutput, int n, int k) {

  // Cria a quantidade de nós heap igual ao número de arquivos temporários
  MinHeapNoh *nohs = new MinHeapNoh[k];
  Atleta *leitura = new Atleta[1];

  stringstream novoNomeArquivo;

  // Para cada nó heap é atribuido o primeiro pacote da struct Atleta do arquivo temporário i
  for (int i = 0; i < k; i++) {
    novoNomeArquivo.str("");
    novoNomeArquivo.clear();
    novoNomeArquivo << "temp_";
    novoNomeArquivo << to_string(i + 1);
    novoNomeArquivo << ".bin";

    ifstream arqBinLeitura(novoNomeArquivo.str(), ios::binary);

    arqBinLeitura.read((char *) (&leitura[0]), sizeof(Atleta));

    nohs[i].atleta = leitura[0];
    nohs[i].i = i;
    nohs[i].j = 1;

    arqBinLeitura.close();
  }

  // Criação do MinHeap com k nohs
  MinHeap hp(nohs, k);

  int qntArquivos = 0;

  int posInsercaoFinal = 0;

  ifstream arqBinEntrada(nomeArqInput, ios::binary);

  arqBinEntrada.close();

  ofstream arqBinResult(nomeArqOutput, ios::binary);

  while (qntArquivos != k) {
    // Cria-se um nó heap que representa a raiz do heap principal
    MinHeapNoh raiz = hp.getMin();

    raiz.alterarPosicao(posInsercaoFinal);

    // Insere na raiz (menor nome ou id {atual} de todos os arquivos temporários) no arquivo final
    arqBinResult.write((char *) (&raiz.atleta), sizeof(Atleta));

    novoNomeArquivo.str("");
    novoNomeArquivo.clear();
    novoNomeArquivo << "temp_";
    novoNomeArquivo << to_string(raiz.i + 1);
    novoNomeArquivo << ".bin";

    ifstream arqBinInput(novoNomeArquivo.str(), ios::binary);
    int qntAtletas = quantidadeAtletas(arqBinInput);


    if (raiz.j < qntAtletas) { // ainda há pacotes a serem tratados
      arqBinInput.seekg(raiz.j * sizeof(Atleta), arqBinInput.beg);

      arqBinInput.read((char *) (&leitura[0]), sizeof(Atleta));

      raiz.atleta = leitura[0];
      raiz.j++;
    } else { // chegou no final do atual arquivo temporário aberto
      // logo, ele é desconsiderado (ficando nos nós mais baixos do MinHeap principal)
      raiz.alterarNome();
      qntArquivos++;
    }
    arqBinInput.close();

    // A raiz atual do MinHeap principal é substituída pelo próximo pacote Atleta do atual arquivo temporário "aberto"
    // O MinHeap é rearranjado (aplica-se o corrigeDescendo) para que o menor nome/id esteja no topo
    hp.inserirMin(raiz);
  }

  arqBinResult.close();
  delete[] leitura;
}

int criarArquivosOrdenados(const string& nomeArqInput, int tamParticao) {
  ifstream arqBinEntrada(nomeArqInput, ios::binary);

  if (arqBinEntrada) {
    int qntAtletas = quantidadeAtletas(arqBinEntrada);

    arqBinEntrada.seekg(0, arqBinEntrada.beg);

    stringstream nomeArqAux;

    Atleta *arrBin = new Atleta[tamParticao];
    Atleta *leitura = new Atleta[1];

    int linhaFim = qntAtletas - (qntAtletas % tamParticao);
    int linhaAtual = 0, cont = 0, total = 0;

    // Lê-se os dados do arquivo original e os insere em arquivos temporários
    while (linhaAtual < linhaFim) {

      arqBinEntrada.read((char *) (&leitura[0]), sizeof(Atleta));

      arrBin[total] = leitura[0];

      total++;

      // arrBin está cheio
      if (total == tamParticao) {

        cont++;

        nomeArqAux.str("");
        nomeArqAux.clear();
        nomeArqAux << "temp_";
        nomeArqAux << to_string(cont);
        nomeArqAux << ".bin";

        // caso queira ver os arquivos sendo criado dinamicamente:
//        cout << nomeArqAux.str() << " criado com sucesso!" << endl;

        // usando quicksort para ordenação interna
        arrBin->qsort(0, total - 1);

        // Salva o arquivo temporário
        salvarArquivo(nomeArqAux.str(), arrBin, tamParticao);

        total = 0;
      }

      linhaAtual++;
    }

    // sobraram dados
    if (linhaAtual != qntAtletas) {
      Atleta *vetorDadosRestantes = new Atleta[qntAtletas - linhaAtual];

      while (linhaAtual < qntAtletas) {
        arqBinEntrada.read((char *) (&leitura[0]), sizeof(Atleta));
        vetorDadosRestantes[total] = leitura[0];

        total++;
        linhaAtual++;
      }

      cont++;

      nomeArqAux.str("");
      nomeArqAux.clear();
      nomeArqAux << "temp_";
      nomeArqAux << to_string(cont);
      nomeArqAux << ".bin";

      // caso queira ver o ultimo arquivo sendo criado dinamicamente:
//      cout << nomeArqAux.str() << " criado com sucesso!" << endl;

      vetorDadosRestantes->qsort(0, total - 1);

      salvarArquivo(nomeArqAux.str(), vetorDadosRestantes, total);

      delete[] vetorDadosRestantes;
    }

    arqBinEntrada.close();
    delete[] leitura;
    delete[] arrBin;
    return cont;
  } else {
    throw runtime_error("Erro! Arquivo não encontrado");
  }
}

void sortExterno(const string& nomeArqInput, const string& nomeArqOutput, int tamParticao) {
  int qntArquivosOrdenados = -1;

  try {
    qntArquivosOrdenados = criarArquivosOrdenados(nomeArqInput, tamParticao);

    cout << "quantidade arquivos temporários: " << qntArquivosOrdenados << endl;

    mergeArquivos(nomeArqInput, nomeArqOutput, tamParticao, qntArquivosOrdenados);
  }
  catch(exception& e){
    cout << e.what() << endl;
  }

  if (qntArquivosOrdenados != -1) {

    stringstream nomeArqAux;

    // deleta todos os arquivos temporarios criado
    for(int i = 0; i < qntArquivosOrdenados; i++){
      nomeArqAux.str("");
      nomeArqAux.clear();
      nomeArqAux << "temp_";
      nomeArqAux << to_string(i + 1);
      nomeArqAux << ".bin";

      remove(nomeArqAux.str().c_str());
    }
    cout << "\nArquivo principal ordenado com sucesso!\n";
  }
}

#endif //PROJETO_ESTRUTURA_DE_DADOS_UFLA_ORDENACAO_HPP
