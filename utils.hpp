#include <iostream>

#include "atleta.hpp"

#ifndef PROJETO_ESTRUTURA_DE_DADOS_UFLA_UTILS_H
#define PROJETO_ESTRUTURA_DE_DADOS_UFLA_UTILS_H
using namespace std;

// constantes utilizadas nos nomes de arquivo binário
const string NOME_ARQ_BINARIO = "data_athlete_info.bin";
const string NOME_ARQ_CSV = "data_athlete_info.csv";
const string NOME_ARQ_SAIDA_CSV = "binToCSV.csv";

// Subprograma no qual sempre orientará o usuário em relação as suas opções de escolha na manipulação dos arquivos;
void printMenu()
{
  cout << "\n|--------------MENU--------------|\n"
       << endl;
  cout << "[0] - Encerrar programa\n";
  cout << "[1] - Remover Atleta\n";
  cout << "[2] - Adicionar Atleta(s)\n";
  cout << "[3] - Buscar\n";
  cout << "[4] - Imprimir Atletas cadastrados\n";
  cout << "[5] - Exportar para CSV\n";
  cout << "[6] - Imprimir um intervalo de Atletas\n";
  cout << "[7] - Trocar 2 atletas de posicao\n";
  cout << "[8] - Adicionar atleta em uma posicao específica\n";
  cout << "[9] - Alterar dados de um registro numa posicao especifica\n";
  cout << "[10] - Importar de arquivo CSV\n";
  cout << "[11] - Ordenar\n\n";
  cout << "\nEscolha uma opcao! > ";
}

int retornaEscolha()
{
  int n;
  cin >> n;
  return n;
}

// Subprograma no qual trabalha em conjunto com as primeiras linhas da função main(); Converte as Strings provenientes do arquivo .csv para int, char e float;
Atleta convertVetor(string vetor[])
{
  Atleta competidor;

  competidor.id = stoi(vetor[0]);
  strcpy(competidor.nome, vetor[1].c_str());
  strcpy(competidor.sex, vetor[2].c_str());
  competidor.idade = int(stof(vetor[3]));
  competidor.altura = stof(vetor[4]);
  competidor.peso = stof(vetor[5]);
  strcpy(competidor.time, vetor[6].c_str());

  return competidor;
}

#endif //PROJETO_ESTRUTURA_DE_DADOS_UFLA_UTILS_H
