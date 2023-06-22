#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>

#include "Binario.hpp"
#include "structs.hpp"
#include "utils.hpp"

using namespace std;

void Binario::ApontarNoFinal() {
  if (this->Aberto()) // se estiver aberto, aponta
  {
    this->arquivoBin.seekg(0, ios::end);
  }
  else // senao, abre o arquivo ja no final
  {
    this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::in | ios::ate);
  }
}

void Binario::MostrarTodos()
{
  this->Abrir();
  int quantAtl = this->QuantidadeDeAtletas();
  this->arquivoBin.seekg(0, fstream::beg);

  Atleta competidorAux;
  for (int i = 0; i < quantAtl; i++)
  {
    this->arquivoBin.seekg(i * sizeof(Atleta));
    this->arquivoBin.read((char *)&competidorAux, sizeof(Atleta));
    if (competidorAux.valido == 1)
    {
      cout << competidorAux.id << "  " << competidorAux.nome << "  " << competidorAux.sex << "  " << competidorAux.idade << "  " << competidorAux.altura << "  " << competidorAux.peso << "  " << competidorAux.time << endl;
    }
  }
  this->Fechar();
}

void Binario::RemoverAtleta(const int &posicao)
{
  this->Abrir();
  int quantAtl = this->QuantidadeDeAtletas();

  this->arquivoBin.seekg(0, this->arquivoBin.beg); // POSICIONA O PONTEIRO DE LEITURA NO INÍCIO DO ARQUIVO
  int cont = posicao - 1;                          // CONTADOR DO LOOP
  Atleta atletaAux;
  if (cont >= 0 and cont < quantAtl)
  {
    this->arquivoBin.seekg(cont * sizeof(Atleta));
    this->arquivoBin.read((char *)&atletaAux, sizeof(Atleta));
    if (atletaAux.valido == 1)
    {
      cout << atletaAux.nome << endl;
      // FAZ A CONVERSÃO DA VARIÁVEL 'VALIDO' PARA 0 E ESCREVE NOVAMENTE O REGISTRO NO ARQUIVO
      atletaAux.valido = 0;
      this->arquivoBin.seekp(cont * sizeof(Atleta));
      this->arquivoBin.write((char *)&atletaAux, sizeof(Atleta));
    }
    else if (atletaAux.valido == 0)
    {
      cout << endl
           << "Atleta ja excluido!" << endl
           << endl;
    }
  }
  else
  {
    cout << "\nNao ha atletas nesta posicao!\n";
  }
  this->Fechar();
}

void Binario::ImportarDeCSVParaBinario(const string &nomeArquivoEntradaCSV)
{
  this->Fechar();
  cout << this->nomeArquivoBin;
  this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::out | ios::in);
  ifstream arqCSV(nomeArquivoEntradaCSV);

  string vetor[7];
  string coluna;
  string aux;
  Atleta competidor;

  getline(arqCSV, aux); // pular a primeira linha (cabecalho)

  while (getline(arqCSV, aux))
  {
    stringstream linha(aux);

    for (int i = 0; i < 7; i++)
    {
      getline(linha, coluna, ',');
      if (coluna.front() == '"' and coluna.back() != '"')
      {
        string colunaTemp;
        while (coluna.back() != '"')
        {
          getline(linha, colunaTemp, ',');
          coluna += ',' + colunaTemp;
        }
      }
      vetor[i] = coluna;
    }

    competidor = convertVetor(vetor); // Até então todos os dados se encontram como string, porém a partir de agora são convertidos para seus devidos tipos por meio desse subprograma;
    this->arquivoBin.write((char *)&competidor, sizeof(Atleta));
  }
  this->Fechar();
  arqCSV.close();
}

void Binario::ExportarParaCSV(string nomeArquivoSaidaCSV)
{
  this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::in | ios::out | ios::ate);

  ofstream arqCSV(nomeArquivoSaidaCSV);

  int quantAtl = this->QuantidadeDeAtletas();
  Atleta atletaAux;

  for (int i = 0; i < quantAtl; i++)
  {
    this->arquivoBin.seekg(i * sizeof(Atleta));
    this->arquivoBin.read((char *)&atletaAux, sizeof(Atleta));

    if (atletaAux.valido != 0)
    {
      arqCSV << atletaAux.id << ";" << atletaAux.nome << ";" << atletaAux.sex << ";" << atletaAux.idade << ";" << atletaAux.altura << ";" << atletaAux.peso << ";" << atletaAux.time << endl;
    }
  }

  arqCSV.close();
  this->Fechar();
}

void Binario::AdicionarAtleta()
{
  Atleta atletaAux;
  int opc;
  cout << "Quantos Atletas deseja cadastrar?" << endl
       << endl;
  cout << "[1] Cadastrar apenas 1." << endl
       << "[2] Mais de um." << endl;
  cin >> opc;
  cout << endl;

  this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::out | ios::in | ios::app);
  int quantAtl = this->QuantidadeDeAtletas();

  switch (opc)
  {
    case 1:
      atletaAux.id = quantAtl + 1;
      cin.ignore();
      cout << "Nome: ";
      cin.getline(atletaAux.nome, 255);
      cout << "Sexo: ";
      cin.getline(atletaAux.sex, 25);
      cout << "Idade: ";
      cin >> atletaAux.idade;
      cin.ignore();
      cout << "Altura: ";
      cin >> atletaAux.altura;
      cin.ignore();
      cout << "Peso: ";
      cin >> atletaAux.peso;
      cin.ignore();
      cout << "Time: ";
      cin.getline(atletaAux.time, 255);

      arquivoBin.write((char *)&atletaAux, sizeof(Atleta));
      break;
    case 2:
      cout << "Quantos ao todo voce deseja cadastrar? ";
      int segOpc;
      cin >> segOpc;
      cout << endl;

      for (int i = 0; i < segOpc; i++)
      {
        cout << "Novo Atleta: " << i + 1 << endl;
        atletaAux.id = quantAtl + 1;
        cin.ignore();
        cout << "Nome: ";
        cin.getline(atletaAux.nome, 255);
        cout << "Sexo: ";
        cin.getline(atletaAux.sex, 25);
        cout << "Idade: ";
        cin >> atletaAux.idade;
        cin.ignore();
        cout << "Altura: ";
        cin >> atletaAux.altura;
        cin.ignore();
        cout << "Peso: ";
        cin >> atletaAux.peso;
        cin.ignore();
        cout << "Time: ";
        cin.getline(atletaAux.time, 255);

        arquivoBin.write((char *)&atletaAux, sizeof(Atleta));

        cout << endl;
      }
      break;
    default:
      cout << "Opcao Invalida";
      break;
  }

  this->Fechar();
}

void Binario::BuscarPorSexo()
{
  this->arquivoBin.open(NOME_ARQ_BINARIO, ios::in | ios::binary | ios::ate);

  // OPERAÇÕES PARA CALCULAR A QUANTIDADE DE REGISTROS PRESENTES NO ARQUIVO BINÁRIO
  int quantAtl = this->QuantidadeDeAtletas();

  cout << "\nDigite o sexo desejado > ";
  cin.clear();
  cin.ignore();
  char sexo[25];
  cin.getline(sexo, 25);
  cout << endl;

  int cont = 0;
  int aux = 0; // CONTADOR DE ATLETAS DE MESMO SEXO
  Atleta competidor;
  this->arquivoBin.seekg(0, this->arquivoBin.beg);

  // REALIZA A BUSCA NO ARQUIVO BINÁRIO
  while (cont < quantAtl)
  {
    this->arquivoBin.seekg(cont * sizeof(Atleta));
    this->arquivoBin.read((char *)&competidor, sizeof(Atleta));
    if (strcmp(sexo, competidor.sex) == 0)
    {
      if (competidor.valido == 1)
      {
        cout << competidor.id << "  " << competidor.nome << "  " << competidor.sex << "  " << competidor.idade << "  " << competidor.altura << "  " << competidor.peso << "  " << competidor.time << endl;
        aux++;
      }
    }
    cont++;
  }

  if (aux == 0)
    cout << "Nao ha Atletas cadastrados com esse sexo!\n";

  this->Fechar();
}

void Binario::BuscarPorNome()
{
  this->arquivoBin.open(NOME_ARQ_BINARIO, ios::in | ios::binary | ios::ate);

  // OPERAÇÕES PARA CALCULAR A QUANTIDADE DE REGISTROS PRESENTES NO ARQUIVO BINÁRIO
  int quantUni = this->QuantidadeDeAtletas();

  cout << "\nDigite o nomeParaProcurar do atleta > ";
  cin.clear();
  cin.ignore();
  char nomeParaProcurar[255];
  cin.getline(nomeParaProcurar, 255);
  cout << endl;

  int cont = 0;
  int aux = 0; // CONTADOR DE ATLETAS COM O MESMO NOME
  Atleta competidor;
  this->arquivoBin.seekg(0, this->arquivoBin.beg);

  // REALIZA A BUSCA NO ARQUIVO BINÁRIO
  while (cont < quantUni)
  {
    this->arquivoBin.seekg(cont * sizeof(Atleta));
    this->arquivoBin.read((char *)&competidor, sizeof(Atleta));
    if (strcmp(nomeParaProcurar, competidor.nome) == 0)
    {
      if (competidor.valido == 1)
      {
        cout << competidor.id << "  " << competidor.nome << "  " << competidor.sex << "  " << competidor.idade << "  " << competidor.altura << "  " << competidor.peso << "  " << competidor.time << endl;
        aux++;
      }
    }
    cont++;
  }

  if (aux == 0)
    cout << "Não ha Atletas cadastrados com esse nomeParaProcurar!\n";

  this->Fechar();
}

void Binario::ImprimirGapDeAtletas()
{
  this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::in | ios::out | ios::ate);
  long int tamArq = this->arquivoBin.tellg();
  int quantAtl = int((tamArq / sizeof(Atleta)));

  cout << "Escolha o intervalo que voce quer imprimir no Arquivo, de 1 a " << quantAtl << ':' << endl
       << endl;
  Atleta competidor;
  int gapInicio, gapFim;
  cin >> gapInicio >> gapFim;
  cin.ignore();
  int cont_atl_validos = (gapFim) - (gapInicio - 1);
  bool verificacao = false;

  if ((gapInicio > 0) and (gapInicio < quantAtl) and (gapFim > 0) and (gapFim < quantAtl))
  {
    cout << endl
         << "Atletas Cadastradas e Ativas entre as linhas " << gapInicio << " - " << gapFim << ":" << endl
         << endl;
    while (gapInicio <= gapFim and cont_atl_validos <= gapFim)
    {
      this->arquivoBin.seekg((gapInicio - 1) * sizeof(Atleta)); //
      this->arquivoBin.read((char *)&competidor, sizeof(Atleta));
      if (competidor.valido == 1)
      {
        cout << competidor.id << "  " << competidor.nome << "  " << competidor.sex << "  " << competidor.idade << "  " << competidor.altura << "  " << competidor.peso << "  " << competidor.time << endl;
        verificacao = true;
      }
      if (verificacao == false)
      {
        cont_atl_validos--; // Quando a verificação/atleta é falsa, ou seja, está "apagada", a cont_atl_validos guarda um lugar para o próximo atleta que é válida/true
        gapFim++;           // O fim é aumentado a fim de percorrer mais uma linha para esse próximo atleta válida/true
      }
      gapInicio++;
      verificacao = false;
    }
  }
  else
    cout << "Intervalo invalido!\n";

  this->Fechar();
}

void Binario::TrocarPosicoes(int pos1, int pos2) {
  this->arquivoBin.open(NOME_ARQ_BINARIO, ios::in | ios::binary | ios::out);
  Atleta aux1, aux2;

  this->arquivoBin.seekg(pos1 * sizeof(Atleta), ios::beg);
  this->arquivoBin.read((char *)&aux1, sizeof(Atleta));
  this->arquivoBin.seekg(pos2 * sizeof(Atleta), ios::beg);
  this->arquivoBin.read((char *)&aux2, sizeof(Atleta));

  this->arquivoBin.seekp(pos2 * sizeof(Atleta), ios::beg);
  this->arquivoBin.write((char *)&aux1, sizeof(Atleta));
  this->arquivoBin.seekp(pos1 * sizeof(Atleta), ios::beg);
  this->arquivoBin.write((char *)&aux2, sizeof(Atleta));

  this->Fechar();
}

void Binario::AdicinoarAtletaEmPosicaoEspecifica(int posicao) {
  this->Abrir();

  // Verificar se a posição é válida
  int quantidadeAtletas = this->QuantidadeDeAtletas();
  if (posicao < 0 || posicao > quantidadeAtletas)
  {
    cout << "Posição inválida." << endl;
    return;
  }

  Atleta atletaAux;

  // Inserindo os dados
  atletaAux.id = this->QuantidadeDeAtletas() + 1;
  cin.ignore();
  cout << "Nome: ";
  cin.getline(atletaAux.nome, 255);
  cout << "Sexo: ";
  cin.getline(atletaAux.sex, 25);
  cout << "Idade: ";
  cin >> atletaAux.idade;
  cin.ignore();
  cout << "Altura: ";
  cin >> atletaAux.altura;
  cin.ignore();
  cout << "Peso: ";
  cin >> atletaAux.peso;
  cin.ignore();
  cout << "Time: ";
  cin.getline(atletaAux.time, 255);

  // Movendo o ponteiro para a posição desejada
  int tamanhoAtleta = sizeof(Atleta);
  streampos posicaoOriginal = this->arquivoBin.tellg();
  streampos posicaoInserir = posicao * tamanhoAtleta;
  this->arquivoBin.seekp(posicaoInserir);

  // Deslocando os elementos posteriores para a direita
  for (int i = quantidadeAtletas - 1; i >= posicao; i--)
  {
    streampos posicaoAtual = i * tamanhoAtleta;
    this->arquivoBin.seekg(posicaoAtual);
    Atleta elemento;
    this->arquivoBin.read((char *)&elemento, tamanhoAtleta);

    streampos proximaPosicao = (i + 1) * tamanhoAtleta;
    this->arquivoBin.seekp(proximaPosicao);
    this->arquivoBin.write((char *)&elemento, tamanhoAtleta);
  }

  // Inserindo o novo atleta na posição desejada
  this->arquivoBin.seekp(posicaoInserir);
  this->arquivoBin.write((char *)&atletaAux, tamanhoAtleta);

  this->Fechar();
}

void Binario::AlterarDadosEmPosicaoEspecifica(int posicao) {
  this->Abrir();

  int quantidadeAtletas = this->QuantidadeDeAtletas();
  if (posicao < 0 || posicao >= quantidadeAtletas)
  {
    cout << "Posição inválida." << endl;
    return;
  }

  // Editando os dados
  Atleta atletaAux;

  cin.ignore();
  cout << "Nome: ";
  cin.getline(atletaAux.nome, 255);
  cout << "Sexo: ";
  cin.getline(atletaAux.sex, 25);
  cout << "Idade: ";
  cin >> atletaAux.idade;
  cin.ignore();
  cout << "Altura: ";
  cin >> atletaAux.altura;
  cin.ignore();
  cout << "Peso: ";
  cin >> atletaAux.peso;
  cin.ignore();
  cout << "Time: ";
  cin.getline(atletaAux.time, 255);

  // Movendo o ponteiro para a posição desejada
  int tamanhoAtleta = sizeof(Atleta);
//  streampos posicaoOriginal = this->arquivoBin.tellg();
  streampos posicaoAlterar = posicao * tamanhoAtleta;
  this->arquivoBin.seekp(posicaoAlterar);

  // Gravando o novo atleta na posição desejada
  this->arquivoBin.write((char *)&atletaAux, tamanhoAtleta);

  this->Fechar();
}
