# projeto-estrutura-de-dados-ufla

Projeto prático feito durante a disciplina de estrutura de dados na ufla 2023-1

## requisitos

- se não tiver, instale o `g++` e `curl`:

```bash
sudo apt install g++ curl
```

- para utilizar o programa, é preciso ter a mesma base de dados que a nossa, seguindo o seguinte padrão:
  - nome do arquivo: `data_athlete_info.csv`
  - para o download do arquivo original da base de dados, clique [aqui](https://drive.google.com/file/d/1Ml9S26KHH65p4lNhMPqG1UkCb6ti0vM8/view).

## Como executar o projeto

- rode `make download_csv` para utilizar o `curl` para baixar a base de dados padrão do projeto
  - caso não queira utilizar este comando, baixe [este arquivo](https://drive.google.com/file/d/1Ml9S26KHH65p4lNhMPqG1UkCb6ti0vM8/view) e extraia-o na raiz do projeto com o nome `data_athlete_info.csv`

```bash
make download_csv
```

- rode `make` para compilar o projeto:

```bash
make
```

- execute o executável criado:

```bash
./programa
```

- caso seja a primeira vez executando, importe o arquivo csv com o comando 10
- [ ] TODO: remover esse comando e importar automaticamente
