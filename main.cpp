#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

// Participantes > Bernardo Bertante Martins, Gustavo Dantas Galote de Lima Padilha , Clarisse Lacerda Pimentel;
// Turma > 10A

// Campos: id; Nome; Idade; Altura; Peso; Time

// Ordenação por Ano e por Pontuação
// Busca por Cidade e por País

const string NOME_ARQ_BINARIO = "data_athlete_info.bin";
const string NOME_ARQ_CSV = "data_athlete_info.csv";
const string NOME_ARQ_SAIDA_CSV = "binToCSV.csv";

struct Atleta {
  int id;
  char nome[255];
  char sex[25];
  int idade;
  float altura;
  float peso;
  char time[255];
  int valido = 1;
};

public:
  Atleta convertVetor(string vetor[]);
  void excluirAtletas(fstream &newArqBi);
  void cadastrarAtletas(Atleta dados);
  int retornaEscolha();
  void buscarPorSexo(fstream &newArqBi);
  void buscarPorNome(fstream &newArqBi);
  void printAtletas(Atleta dados);
  void exportarCSV(fstream &arqBi);
  void imprimeGap(fstream &arqBi);
  void trocarAtletas(fstream &newArqBi);
};

// Prototipagem dos subprogramas na ordem em que aparecem depois do int main
Atleta convertVetor(string vetor[]);
void printMenu();
void excluirAtletas(fstream &newArqBi);
void cadastrarAtletas(Atleta dados);
int retornaEscolha();
void buscarPorSexo(fstream &newArqBi);
void buscarPorNome(fstream &newArqBi);
void printAtletas(Atleta competidor);
void exportarCSV(fstream &arqBi);
void imprimeGap(fstream &arqBi);
void trocarAtletas(fstream &newArqBi);

int main()
{

  // Aqui começa a conversão dos dados do arquivo .csv para o arquivo binário;

  ifstream arqCSV("data_athlete_info.csv");
  fstream newArqBi;
  newArqBi.open("rankToBi.bin", ios::binary | ios::out | ios::in);
  string vetor[7];
  string coluna;
  string aux;
  Atleta competidor;

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

    competidor = competidor.convertVetor(vetor); // Até então todos os dados se encontram como string, porém a partir de agora são convertidos para seus devidos tipos por meio desse subprograma;
    newArqBi.write((char *)&competidor, sizeof(Atleta));
  }
  newArqBi.close();

  cout << "Arquivo gerado com sucesso!" << endl;

  arqCSV.close();

  competidor.printAtletas(competidor); // Mostra as Faculdades já cadastradas assim que o programa é inicializado;

  // Os dados iniciais na abetura atual do programa são mostrados logo de cara para o usuáio, para que o mesmo tenha noção do estado corrente do arquivo binário;

  int opc; // varável do switch

  // MENU DE OPÇÕES COM CHAMADA DA FUNÇÃO printMenu();
  do
  {
    // printUniversidades(dados);
    printMenu();
    /*[0] - Encerrar programa
      [1] - Ordenar arquivo
      [2] - Remover Atleta
      [3] - Adicionar Atleta(s)
      [4] - Buscar
      [5] - Imprimir Atletas cadastrados
      [6] - Converter Binário para CSV
      [7] - Imprimir um intervalo de Atletas
      */
    cin >> opc;
    cout << "----------------------------\n";

    int opt = 0; //  variável que receberá o retorno da função retornaEscolha();

    switch (opc) // Dentro desse switch principal, dependendo de qual número o usuário digitar, diferentes funções são acionadas! Cada um com seus respctivos objetivos.
    {
    case 0:
      break;
    case 1:
      newArqBi.open("rankToBi.bin", ios::in | ios::binary | ios::out);
      competidor.excluirAtletas(newArqBi);
      newArqBi.close();
      break;
    case 2:
      cout << "Cadastrar novo(s) Atletas:\n"
           << endl;
      competidor.cadastrarAtletas(competidor);
      break;
    case 3:
      newArqBi.open("rankToBi.bin", ios::in | ios::binary | ios::ate);
      cout << "[1] - Buscar por Sexo\n[2] - Busca por Nome\nOu pressione qualquer numero para voltar ao menu anterior\n> ";
      opt = competidor.retornaEscolha();
      if (opt == 1)
      {
        competidor.buscarPorSexo(newArqBi);
      }
      else if (opt == 2)
      {
        competidor.buscarPorNome(newArqBi);
      }
      newArqBi.close();
      break;
    case 4:
      cout << "Atletas Cadastrados e Ativos:" << endl
           << endl;
      competidor.printAtletas(competidor);
      break;
    case 5:
      cout << endl
           << "Arquivo CSV exportado!" << endl;
      newArqBi.open("rankToBi.bin", ios::binary | ios::in | ios::out | ios::ate);
      competidor.exportarCSV(newArqBi);
      newArqBi.close();
      break;
    case 6:
      newArqBi.open("rankToBi.bin", ios::binary | ios::in | ios::out | ios::ate);
      competidor.imprimeGap(newArqBi);
      newArqBi.close();
      break;
    case 7:
      newArqBi.open("rankToBi.bin", ios::in | ios::binary | ios::out);
      competidor.trocarAtletas(newArqBi);
      newArqBi.close();
      break;
    default:
      cout << "DIGITE UMA OPCAO VALIDA!\n";
      break;
    }
  } while (opc != 0);

  return 0;
}

Atleta Atleta::convertVetor(string vetor[]) // Subprograma no qual trabalha em conjunto com as primeiras linhas da função main(); Converte as Strings provenientes do arquivo .csv para int, char e float;
{
  Atleta competidor;

  competidor.id = stoi(vetor[0]);
  strcpy(competidor.nome, vetor[1].c_str());
  strcpy(competidor.sex, vetor[2].c_str());
  competidor.idade = stoi(vetor[3]);
  competidor.altura = stof(vetor[4]);
  competidor.peso = stof(vetor[5]);
  strcpy(competidor.time, vetor[6].c_str());

  return competidor;
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
  cout << "[7] - Trocar 2 atletas de posicao\n"
       << endl;
  cout << "\nEscolha uma opcao! > ";
}

void Atleta::excluirAtletas(fstream &newArqBi) // SUBPROGRAMA QUE EXCLUI UM ATLETA DESEJADO
{
  newArqBi.seekg(0, ios::end);
  int quantAtl = newArqBi.tellg() / sizeof(Atleta);
  Atleta competidor;

  cout << "\nDigite a posicao do Atleta a ser excluido (1 - " << quantAtl << ")\n> ";
  int posicao;
  cin >> posicao;

  bool verificacao = false;
  newArqBi.seekg(0, newArqBi.beg); // POSICIONA O PONTEIRO DE LEITURA NO INÍCIO DO ARQUIVO
  int cont = posicao - 1;          // CONTADOR DO LOOP
  if (cont >= 0 and cont < quantAtl)
  {
    newArqBi.seekg(cont * sizeof(Atleta));
    newArqBi.read((char *)&competidor, sizeof(Atleta));
    if (competidor.valido == 1)
    {
      cout << competidor.nome << endl;
      // FAZ A CONVERSÃO DA VARIÁVEL 'VALIDO' PARA 0 E ESCREVE NOVAMENTE O REGISTRO NO ARQUIVO
      competidor.valido = 0;
      newArqBi.seekp(cont * sizeof(Atleta));
      newArqBi.write((char *)&competidor, sizeof(Atleta));
      verificacao = true;
    }
    else if (verificacao == false)
      cout << endl
           << "Atleta ja excluido!" << endl
           << endl;
  }
  else
    cout << "\nNao ha atletas nesta posicao!\n";

  newArqBi.close();
}

void Atleta::cadastrarAtletas(Atleta competidor) // Subprograma no qual permite o usuário adicionar 1 ou Mais Atletas ao arquivo binário quando chamado;

{
  int opc;
  cout << "Quantos Atletas deseja cadastrar?" << endl
       << endl;
  cout << "[1] Cadastrar apenas 1." << endl
       << "[2] Mais de um." << endl;
  cin >> opc;
  cout << endl;

  fstream novoCadastro;
  novoCadastro.open("rankToBi.bin", ios::binary | ios::out | ios::in | ios::app);
  int quantAtl = novoCadastro.tellg() / sizeof(Atleta);

  switch (opc)
  {
  case 1:
    competidor.id = quantAtl + 1;
    cin.ignore();
    cout << "Nome: ";
    cin.getline(competidor.nome, 255);
    cout << "Sexo: ";
    cin.getline(competidor.sex, 25);
    cout << "Idade: ";
    cin >> competidor.idade;
    cin.ignore();
    cout << "Altura: ";
    cin >> competidor.altura;
    cin.ignore();
    cout << "Peso: ";
    cin >> competidor.peso;
    cin.ignore();
    cout << "Time: ";
    cin.getline(competidor.time, 255);

    novoCadastro.write((char *)&competidor, sizeof(Atleta));
    break;
  case 2:
    cout << "Quantos ao todo voce deseja cadastrar? ";
    int segOpc;
    cin >> segOpc;
    cout << endl;

    for (int i = 0; i < segOpc; i++)
    {
      cout << "Novo Atleta: " << i + 1 << endl;
      competidor.id = quantAtl + 1;
      cin.ignore();
      cout << "Nome: ";
      cin.getline(competidor.nome, 255);
      cout << "Sexo: ";
      cin.getline(competidor.sex, 25);
      cout << "Idade: ";
      cin >> competidor.idade;
      cin.ignore();
      cout << "Altura: ";
      cin >> competidor.altura;
      cin.ignore();
      cout << "Peso: ";
      cin >> competidor.peso;
      cin.ignore();
      cout << "Time: ";
      cin.getline(competidor.time, 255);

      novoCadastro.write((char *)&competidor, sizeof(Atleta));

      cout << endl;
    }
    break;
  default:
    cout << "Opcao Invalida";
    break;
  }

  novoCadastro.close();
}

void Atleta::trocarAtletas(fstream &newArqBi) // Subprograma que altera 2 posicoes/atletas entre si
{
  Atleta aux1, aux2;

  int esc1, esc2;
  cout << "Digite as 2  posicoes em sequencia para efetuar a troca: ";
  cin >> esc1;
  cin >> esc2;

  newArqBi.seekg(esc1 * sizeof(Atleta), ios::beg);
  newArqBi.read((char *)&aux1, sizeof(Atleta));
  newArqBi.seekg(esc2 * sizeof(Atleta), ios::beg);
  newArqBi.read((char *)&aux2, sizeof(Atleta));

  newArqBi.seekp(esc2 * sizeof(Atleta), ios::beg);
  newArqBi.write((char *)&aux1, sizeof(Atleta));
  newArqBi.seekp(esc1 * sizeof(Atleta), ios::beg);
  newArqBi.write((char *)&aux2, sizeof(Atleta));
}

int Atleta::retornaEscolha()
{
  int n;
  cin >> n;
  return n;
}

void Atleta::buscarPorSexo(fstream &newArqBi) // Subprograma que tem a função de fazer a busca de determinad sexo expecificada pelo usuário dentro do arquivo binário;
{
  // OPERAÇÕES PARA CALCULAR A QUANTIDADE DE REGISTROS PRESENTES NO ARQUIVO BINÁRIO
  long int tamArq = newArqBi.tellg();
  int quantAtl = int(tamArq / sizeof(Atleta));

  cout << "\nDigite o sexo desejado > ";
  cin.clear();
  cin.ignore();
  char sexo[25];
  cin.getline(sexo, 25);
  cout << endl;

  int cont = 0;
  int aux = 0; // CONTADOR DE ATLETAS DE MESMO SEXO
  Atleta competidor;
  newArqBi.seekg(0, newArqBi.beg);

  // REALIZA A BUSCA NO ARQUIVO BINÁRIO
  while (cont < quantAtl)
  {
    newArqBi.seekg(cont * sizeof(Atleta));
    newArqBi.read((char *)&competidor, sizeof(Atleta));
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

  newArqBi.close();
}

void Atleta::buscarPorNome(fstream &newArqBi) // Subprograma que tem a função de fazer a busca de determinado nome expecificado pelo usuário dentro do arquivo binário;
{
  // OPERAÇÕES PARA CALCULAR A QUANTIDADE DE REGISTROS PRESENTES NO ARQUIVO BINÁRIO
  long int tamArq = newArqBi.tellg();
  int quantUni = int(tamArq / sizeof(Atleta));

  cout << "\nDigite o nome do atleta > ";
  cin.clear();
  cin.ignore();
  char nome[255];
  cin.getline(nome, 255);
  cout << endl;

  int cont = 0;
  int aux = 0; // CONTADOR DE ATLETAS COM O MESMO NOME
  Atleta competidor;
  newArqBi.seekg(0, newArqBi.beg);

  // REALIZA A BUSCA NO ARQUIVO BINÁRIO
  while (cont < quantUni)
  {
    newArqBi.seekg(cont * sizeof(Atleta));
    newArqBi.read((char *)&competidor, sizeof(Atleta));
    if (strcmp(nome, competidor.nome) == 0)
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
    cout << "Não ha Atletas cadastrados com esse nome!\n";

  newArqBi.close();
}

void Atleta::printAtletas(Atleta competidor) // Subprograma que exibe/printa no terminal o atual estado(informações) do arquivo binário, porém em decimal;
{
  ifstream arqBi;
  arqBi.open("rankToBi.bin", ios::binary | ios::in | ios::ate);
  long int tamArq = arqBi.tellg();
  int quantAtl = int(tamArq / sizeof(Atleta));

  int cont = 0;
  arqBi.seekg(0, arqBi.beg);
  while (cont < quantAtl)
  {
    arqBi.seekg(cont * sizeof(Atleta));
    arqBi.read((char *)&competidor, sizeof(Atleta));
    if (competidor.valido == 1)
    {
      cout << competidor.id << "  " << competidor.nome << "  " << competidor.sex << "  " << competidor.idade << "  " << competidor.altura << "  " << competidor.peso << "  " << competidor.time << endl;
    }
    cont++;
  }
  arqBi.close();
}

void Atleta::exportarCSV(fstream &arqBi) // Subprograma na qual sua função é de converter o atual arquivo binário para um arquivo novo .csv;
{
  ofstream arqCSV("binToCSV.csv");

  long int tamArq = arqBi.tellg();
  int quantAtl = int(tamArq / sizeof(Atleta));
  int cont = 0;
  Atleta competidor;

  while (cont < quantAtl)
  {
    arqBi.seekg(cont * sizeof(Atleta));
    arqBi.read((char *)&competidor, sizeof(Atleta));

    if (competidor.valido != 0)
    {
      arqCSV << competidor.id << ";" << competidor.nome << ";" << competidor.sex << ";" << competidor.idade << ";" << competidor.altura << ";" << competidor.peso << ";" << competidor.time << endl;
    }

    cont++;
  }
  arqCSV.close();
}

void Atleta::imprimeGap(fstream &arqBi) // SUBPROGRAMA QUE IMPRIME UM INTERVALO DE ATLETAS
{
  long int tamArq = arqBi.tellg();
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
         << "Universidades Cadastradas e Ativas entre as linhas " << gapInicio << " - " << gapFim << ":" << endl
         << endl;
    while (gapInicio <= gapFim and cont_atl_validos <= gapFim)
    {
      arqBi.seekg((gapInicio - 1) * sizeof(Atleta)); //
      arqBi.read((char *)&competidor, sizeof(Atleta));
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
}