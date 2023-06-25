#include <iostream>
#include <exception>

#include "utils.hpp"
#include "structs.hpp"
#include "Binario.hpp"

using namespace std;

// Participantes > Bernardo Bertante Martins, Gustavo Dantas Galote de Lima Padilha, Clarisse Lacerda Pimentel;
// Turma > 10A

// Campos: id; Nome; Idade; Altura; Peso; Time

// Busca por Nome e por Sexo

int main() {
  try {
    Binario bin(NOME_ARQ_BINARIO);

    // caso seja a primeira vez rodando o projeto (binário nao existe)
    if (!bin.Existe()) {
      cout << "Aguarde enquanto importamos o arquivo csv pela primeira vez!" << endl;
      bin.ImportarDeCSVParaBinario(NOME_ARQ_CSV);
      cout << "Arquivo importado com sucesso!" << endl;
    }

    bin.Fechar();

    int opc; // varável do switch

    do {
      try {
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
            if (opt == 1) {
              bin.BuscarPorSexo();
            } else if (opt == 2) {
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
            cout << "Digite as 2 posicoes em sequencia para efetuar a troca, de 1 a " << bin.QuantidadeDeAtletas()
                 << ':'
                 << endl
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
          case 10: // Ordenação
            bin.Ordenar();
            break;
          case 11:
            // 🤫 apenas mostra a quantidade de atletas cadastrados, segredo
            cout << endl << bin.QuantidadeDeAtletas() << endl;
            break;
          default:
            cout << "Digite uma opcao valida!\n";
            break;
        }
      } catch (exception &e) {
        cout << endl << e.what() << endl;
      }
    } while (opc != 0);
  } catch (exception& e) {
    cout << endl << e.what() << endl;
    return 1;
  }
  return 0;
}