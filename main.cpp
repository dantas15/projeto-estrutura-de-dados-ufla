#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

// Participantes > Bernardo Bertante Martins, Gustavo Dantas Galote de Lima Padilha, Clarisse Lacerda Pimentel;
// Turma > 10A

// Campos: id; Nome; Idade; Altura; Peso; Time

// Busca por Nome e por Sexo

const string NOME_ARQ_BINARIO = "data_athlete_info.bin";
const string NOME_ARQ_CSV = "data_athlete_info.csv";
const string NOME_ARQ_SAIDA_CSV = "binToCSV.csv";

struct Atleta
{
  int id;
  char nome[255];
  char sex[25];
  int idade;
  float altura;
  float peso;
  char time[255];
  int valido = 1;
};

class Binario
{
private:
  string nomeArquivoBin;
  fstream arquivoBin;

public:
  Binario(string nomeArquivoBinario);
  ~Binario()
  {
    if (this->arquivoBin.is_open())
    {
      this->arquivoBin.close();
    }
  };
  inline bool Abrir()
  {
    this->Fechar();
    this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::in | ios::out);
    return arquivoBin.is_open();
  }
  inline void Fechar()
  {
    if (this->Aberto())
    {
      this->arquivoBin.close();
    }
  }
  inline bool Aberto()
  {
    return this->arquivoBin.is_open();
  }
  inline int QuantidadeDeAtletas()
  {
    int qnt;
    if (this->Aberto())
    {
      // armazenando o ponteiro incial antes de ir pro final
      streampos posicaoOriginal = this->arquivoBin.tellg();
      this->ApontarNoFinal();
      qnt = this->arquivoBin.tellg() / sizeof(Atleta);

      // voltando pro ponteiro original
      this->arquivoBin.seekg(posicaoOriginal);
    }
    else
    {
      this->Abrir();
      this->ApontarNoFinal();
      qnt = this->arquivoBin.tellg() / sizeof(Atleta);
      this->Fechar();
    }
    return qnt;
  }
  void ApontarNoFinal()
  {
    if (this->Aberto()) // se estiver aberto, aponta
    {
      this->arquivoBin.seekg(0, ios::end);
    }
    else // senao, abre o arquivo ja no final
    {
      this->arquivoBin.open(this->nomeArquivoBin, ios::binary | ios::in | ios::ate);
    }
  }
  // TODO Ordenar();
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
};

// Prototipagem dos subprogramas na ordem em que aparecem depois do main()
int retornaEscolha();
Atleta convertVetor(string vetor[]); // Subprograma no qual trabalha em conjunto com as primeiras linhas da função main(); Converte as Strings provenientes do arquivo .csv para int, char e float;
void printMenu();                    // Subprograma no qual sempre orientará o usuário em relação as suas opções de escolha na manipulação dos arquivos;

int main()
{
  Binario bin(NOME_ARQ_BINARIO);
  bin.Abrir();
  bin.Fechar();

  int opc; // varável do switch

  do
  {
    printMenu();

    cin >> opc;
    cout << "----------------------------\n";

    int opt = 0; //  variável que receberá o retorno da função retornaEscolha();

    switch (opc) // Dentro desse switch principal, dependendo de qual número o usuário digitar, diferentes funções são acionadas! Cada um com seus respctivos objetivos.
    {
    case 0: // teste
      break;
    case 1: // Remover
      int posicao;
      cout << "\nDigite a posicao do Atleta a ser excluido (1 - " << bin.QuantidadeDeAtletas() << ")\n> ";
      cin >> posicao;
      bin.RemoverAtleta(posicao);
      break;
    case 2: // Cadastrar
      cout << "Cadastrar novo(s) Atletas:\n"
           << endl;
      bin.AdicionarAtleta();
      break;
    case 3: // Buscar
      cout
          << "[1] - Buscar por Sexo\n[2] - Busca por Nome\nOu pressione qualquer numero para voltar ao menu anterior\n> ";
      opt = retornaEscolha();
      if (opt == 1)
      {
        bin.BuscarPorSexo();
      }
      else if (opt == 2)
      {
        bin.BuscarPorNome();
      }
      bin.Fechar();
      break;
    case 4: // Imprimir Cadastrados
      cout << "Atletas Cadastrados e Ativos:" << endl
           << endl;
      bin.MostrarTodos();
      break;
    case 5: // Exportar para CSV
      bin.ExportarParaCSV(NOME_ARQ_SAIDA_CSV);
      cout << endl
           << "Arquivo CSV exportado!" << endl;
      break;
    case 6: // Imprimir intervalo (gap) de atletas
      bin.ImprimirGapDeAtletas();
      break;
    case 7: // Trocar 2 atletas de posicao
      int pos1, pos2;
      cout << "Digite as 2 posicoes em sequencia para efetuar a troca, de 1 a " << bin.QuantidadeDeAtletas() << ':' << endl
           << endl;
      cin >> pos1 >> pos2;
      bin.TrocarPosicoes(pos1, pos2);
      break;
    case 8: // Adicionar atleta em uma posicao específica
      int pos;
      cout << "Digite a posicao que deseja efetua a troca, de 1 a " << bin.QuantidadeDeAtletas() << ':' << endl
           << endl;
      cin >> pos;
      bin.AdicinoarAtletaEmPosicaoEspecifica(pos);
      break;
    case 9: // Alterar dados de um registro numa posicao especifica
      int posEditar;
      cout << "Digite a posicao que deseja efetua a troca, de 1 a " << bin.QuantidadeDeAtletas() << ':' << endl
           << endl;
      cin >> posEditar;
      bin.AlterarDadosEmPosicaoEspecifica(posEditar);
      break;
    case 10: // Importar de arquivo CSV
      bin.ImportarDeCSVParaBinario(NOME_ARQ_CSV);
      break;
    default:
      cout << "DIGITE UMA OPCAO VALIDA!\n";
      break;
    }
  } while (opc != 0);

  return 0;
}

void printMenu() // Subprograma no qual sempre orientará o usuário em relação as suas opções de escolha na manipulação dos arquivos;
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
  cout << "[10] - Importar de arquivo CSV\n\n";
  cout << "\nEscolha uma opcao! > ";
}

int retornaEscolha()
{
  int n;
  cin >> n;
  return n;
}

Atleta convertVetor(string vetor[]) // Subprograma no qual trabalha em conjunto com as primeiras linhas da função main(); Converte as Strings provenientes do arquivo .csv para int, char e float;
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

Binario::Binario(std::string nomeArquivoBinario)
{
  this->nomeArquivoBin = nomeArquivoBinario;
  fstream arquivoBin(nomeArquivoBinario);
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
  cout << "Arquivo importado com sucesso!" << endl;
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

void Binario::TrocarPosicoes(int pos1, int pos2) // Subprograma que altera 2 posicoes/atletas entre si
{
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

void Binario::AdicinoarAtletaEmPosicaoEspecifica(int posicao)
{
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

void Binario::AlterarDadosEmPosicaoEspecifica(int posicao)
{
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
  streampos posicaoOriginal = this->arquivoBin.tellg();
  streampos posicaoAlterar = posicao * tamanhoAtleta;
  this->arquivoBin.seekp(posicaoAlterar);

  // Gravando o novo atleta na posição desejada
  this->arquivoBin.write((char *)&atletaAux, tamanhoAtleta);

  this->Fechar();
}
