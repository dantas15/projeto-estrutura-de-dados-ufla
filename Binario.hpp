#include <iostream>
#include <fstream>

#include "structs.hpp"

#ifndef PROJETO_ESTRUTURA_DE_DADOS_UFLA_BINARIO_H
#define PROJETO_ESTRUTURA_DE_DADOS_UFLA_BINARIO_H
using namespace std;

class Binario
{
private:
    string nomeArquivoBin;
    fstream arquivoBin;
public:
    Binario(string nomeArquivoBinario) {
      nomeArquivoBin = nomeArquivoBinario;
    }
    ~Binario() {
      if (arquivoBin.is_open())
      {
        arquivoBin.close();
      }
    }
    inline bool Existe() {
      ifstream arqexiste (nomeArquivoBin);
      bool existe = arqexiste.good();
      arqexiste.close();
      return existe;
    }
    inline bool Abrir() {
      Fechar();
      arquivoBin.open(nomeArquivoBin, ios::binary | ios::in | ios::out);
      if(!arquivoBin.good()) {
        throw runtime_error("Erro! O arquivo não existe, importe-o primeiro!");
      }

      return arquivoBin.is_open();
    }
    inline void Fechar() {
      if (Aberto())
      {
        arquivoBin.close();
      }
    }
    inline bool Aberto() {
      return arquivoBin.is_open();
    }
    inline int QuantidadeDeAtletas() {
      int qnt;
      if (Aberto())
      {
        // armazenando o ponteiro incial antes de ir pro final
        streampos posicaoOriginal = arquivoBin.tellg();
        ApontarNoFinal();
        qnt = arquivoBin.tellg() / sizeof(Atleta);

        // voltando pro ponteiro original
        arquivoBin.seekg(posicaoOriginal);
      }
      else
      {
        Abrir();
        ApontarNoFinal();
        qnt = arquivoBin.tellg() / sizeof(Atleta);
        Fechar();
      }
      return qnt;
    }
    void ApontarNoFinal();
    void MostrarTodos();
    void RemoverAtleta(const int &posicao);
    void AdicionarAtleta();
    void BuscarPorSexo();
    void BuscarPorNome();
    void TrocarPosicoes(int pos1, int pos2);
    void ExportarParaCSV(string nomeArquivoSaidaCSV);
    void ImportarDeCSVParaBinario(const string &nomeArquivoEntradaCSV);
    void ImprimirGapDeAtletas();
    void AdicinoarAtletaEmPosicaoEspecifica(int posicao);
    void AlterarDadosEmPosicaoEspecifica(int posicao);
    void Ordenar();
};
#endif //PROJETO_ESTRUTURA_DE_DADOS_UFLA_BINARIO_H
