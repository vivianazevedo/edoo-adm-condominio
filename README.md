# Sistema de Gestão de Condomínio

Sistema de informação em **C++ orientado a objetos** para administrar um condomínio: apartamentos, moradores, funcionários, visitantes, áreas comuns e reservas. Os dados ficam em um banco **SQLite** e o sistema pode ser usado por uma **interface gráfica (Qt 6)** ou pelo **terminal**.

> Projeto prático da disciplina **CIN0135 – Estruturas de Dados Orientadas a Objetos**
> Centro de Informática (CIn) – UFPE · Semestre 2026.2 · Prof. Francisco Paulo Magalhães Simões

- **Página do projeto:** [vivianazevedo.github.io/edoo-adm-condominio](https://vivianazevedo.github.io/edoo-adm-condominio/)
- **Vídeo de apresentação:** https://drive.google.com/drive/folders/1Gb5bE8oEQPiTGzc9N_Ti4D7jimusW1LI?usp=sharing
- **Relatório:** [`docs/relatorio.pdf`](docs/relatorio.pdf)

## Equipe

Vivian Azevedo · Clécio Muniz · Sâmia Freitas · Beatriz Luna

---

## Sumário

1. [Funcionalidades](#funcionalidades)
2. [Como rodar o projeto](#como-rodar-o-projeto)
3. [Como usar](#como-usar)
4. [Regras de negócio](#regras-de-negócio)
5. [Arquitetura](#arquitetura)
6. [Conceitos de POO aplicados](#conceitos-de-poo-aplicados)
7. [Banco de dados](#banco-de-dados)
8. [Estrutura do repositório](#estrutura-do-repositório)
9. [Documentação](#documentação)
10. [Limitações e trabalhos futuros](#limitações-e-trabalhos-futuros)
11. [Fluxo de trabalho em equipe](#fluxo-de-trabalho-em-equipe)
12. [Referências](#referências)

---

## Funcionalidades

- **Apartamentos:** cadastro, listagem, edição e remoção (CRUD completo).
- **Moradores:** cadastro vinculado a um apartamento, com tipo de ocupação (proprietário, inquilino ou dependente), listagem geral ou por apartamento, edição e remoção.
- **Funcionários:** CRUD completo, com cargo (porteiro, zelador, faxineiro ou administrador), turno e data de admissão.
- **Visitantes e visitas:** cadastro de visitantes, registro de entrada e de saída por um porteiro, e histórico de visitas por apartamento.
- **Áreas comuns:** salão de festas, piscina e churrasqueira, cada uma com capacidade, taxa-base e horário de funcionamento. Na interface gráfica há cadastro, edição (nome, capacidade e taxa) e remoção; o terminal lista e cadastra.
- **Reservas:** criar e cancelar, com validação de conflito de horário, capacidade, duração e horário de funcionamento, e cálculo automático do valor. A consulta por morador está no terminal; a interface gráfica lista todas as reservas.

Há duas formas de usar o sistema, ambas sobre o mesmo núcleo e o mesmo banco:

- `condominio_gui`: interface gráfica Qt 6 com três abas (Moradores e Aptos, Áreas e Reservas, Portaria e Visitas). Os funcionários são gerenciados na aba Portaria e Visitas.
- `condominio_terminal`: interface em menus no terminal (apartamentos, moradores e funcionários; áreas e reservas; portaria com visitantes e visitas).

---

## Como rodar o projeto

### 1. Pré-requisitos

| Ferramenta | Para quê | Obrigatório? |
|---|---|---|
| Compilador C++17 (GCC, Clang ou MSVC) | compilar o código | sim |
| [CMake](https://cmake.org/) 3.16 ou superior | configurar e compilar | sim |
| [Qt 6](https://www.qt.io/download) (módulo Widgets) | interface gráfica | só para a GUI |

O SQLite já vem dentro do repositório (`third_party/sqlite`), então **não precisa instalar nada** para o banco.

Se o Qt 6 não for encontrado, o CMake avisa e compila apenas o núcleo, o terminal e os testes. A GUI só é gerada quando o Qt está instalado.

**macOS (Homebrew)**

```bash
xcode-select --install     # compilador, se ainda não tiver
brew install cmake qt
```

**Linux (Ubuntu/Debian)**

```bash
sudo apt install build-essential cmake qt6-base-dev
```

**Windows**

Instale o Visual Studio (com o componente "Desenvolvimento para desktop com C++"), o CMake e o Qt 6 para a mesma versão do compilador. Use o **Developer Command Prompt** do Visual Studio para os comandos abaixo.

### 2. Clonar o repositório

```bash
git clone https://github.com/vivianazevedo/edoo-adm-condominio
cd edoo-adm-condominio
```

### 3. Compilar

**macOS**

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
cmake --build build -j4
```

**Linux**

```bash
cmake -S . -B build
cmake --build build -j4
```

**Windows (Developer Command Prompt)**

```powershell
cmake -S . -B build -G "NMake Makefiles" -DCMAKE_PREFIX_PATH="CAMINHO_DA_INSTALACAO_DO_QT"
cmake --build build
```

No final da configuração do CMake, **não** pode aparecer a mensagem `Qt 6 Widgets nao encontrado` se você quer a interface gráfica. Se aparecer, informe a pasta do Qt em `-DCMAKE_PREFIX_PATH`.

### 4. Executar

Rode sempre de **dentro da pasta `build`**: é lá que o programa encontra o `sql/schema.sql` (o CMake copia a pasta `sql/` para o build) e é onde o arquivo `condominio.db` é criado.

```bash
cd build
./condominio_gui          # interface gráfica (Linux e macOS)
./condominio_terminal     # versão no terminal
```

No Windows: `condominio_gui.exe` e `condominio_terminal.exe`. As DLLs do Qt precisam estar no `PATH` para a GUI abrir.

Na primeira execução, o banco `condominio.db` é criado automaticamente a partir de `sql/schema.sql`. Para recomeçar do zero, feche o programa e apague o arquivo `condominio.db`. Para reencontrar seus dados, execute sempre a partir da mesma pasta.

### 5. Rodar os testes

Na raiz do projeto:

```bash
ctest --test-dir build --output-on-failure
```

São 14 testes (13 se o Qt não estiver instalado), cobrindo banco, repositórios, fábricas, serviços, correções de bugs, fluxos da GUI e cenários de integração. O resultado esperado é `100% tests passed`. Detalhes em [`docs/testes.md`](docs/testes.md).

### 6. Dados de exemplo (opcional)

O arquivo `sql/seed.sql` tem dados fictícios (um apartamento, um morador, um porteiro, um visitante, uma área, uma reserva e uma visita) para demonstração. Ele **não** é aplicado automaticamente. Use apenas em um banco vazio, depois de abrir o sistema uma vez para criar as tabelas. É preciso ter o cliente de linha de comando `sqlite3` instalado (ele não faz parte dos pré-requisitos do projeto):

```bash
cd build
sqlite3 condominio.db < ../sql/seed.sql
```

### Problemas comuns

| Sintoma | Causa provável | O que fazer |
|---|---|---|
| `Qt 6 Widgets nao encontrado` | O CMake não achou o Qt | Passe `-DCMAKE_PREFIX_PATH` com a pasta de instalação do Qt (no macOS: `$(brew --prefix qt)`) e apague a pasta `build` antes de configurar de novo |
| `Falha ao iniciar o sistema` | O programa não encontrou o `sql/schema.sql` | Execute de dentro da pasta `build` |
| Os dados sumiram | O `condominio.db` foi criado em outra pasta | Execute sempre a partir da mesma pasta |
| A janela não abre no Windows | DLLs do Qt fora do `PATH` | Adicione a pasta `bin` do Qt ao `PATH` |

Instruções complementares em [`docs/execucao-local.md`](docs/execucao-local.md) e [`docs/interface-grafica.md`](docs/interface-grafica.md).

---

## Como usar

A ordem importa, porque cada cadastro depende do anterior.

1. **Moradores e Aptos:** cadastre primeiro um **apartamento** e depois um **morador** nele. Clique numa linha da tabela para preencher o formulário antes de editar ou remover.
2. **Portaria e Visitas:** cadastre um **funcionário com cargo Porteiro** e um **visitante**. Selecione visitante, apartamento e porteiro para registrar a entrada. Escolha a visita na tabela para registrar a saída. O filtro mostra o histórico por apartamento.
3. **Áreas e Reservas:** cadastre uma **área comum**, escolha o **morador** e reserve um horário. Se o horário conflitar, ultrapassar a capacidade ou estiver fora do funcionamento, o sistema recusa e explica o motivo.

Os erros das regras de negócio aparecem em janelas explicativas, e os dados são atualizados ao trocar de aba.

---

## Regras de negócio

As regras ficam na camada de serviço e no modelo, e são verificadas pelos testes automáticos.

**Reservas**

- Não é possível reservar em data ou horário que já passou.
- Não é possível reservar uma área em horário que conflite com outra reserva ativa.
- A reserva respeita a capacidade, o horário de funcionamento e a duração máxima de cada área:

| Área | Duração máxima | Valor da reserva |
|---|---|---|
| Salão de festas | 8 h | taxa-base + R$ 3,00 por convidado |
| Piscina | 2 h (máx. 4 convidados) | gratuita |
| Churrasqueira | 4 h | R$ 40,00 fixos |

- O cancelamento exige **24 horas de antecedência**, e uma reserva já cancelada não pode ser cancelada de novo.

**Visitas**

- Somente um funcionário com cargo de **porteiro** registra entrada de visitantes.
- Uma visita só recebe uma saída, e a saída não pode ser anterior à entrada.

**Cadastros**

- O **CPF** precisa ser válido (11 dígitos com dígitos verificadores) e é **único** entre todas as pessoas (moradores, funcionários e visitantes).
- Não pode haver dois apartamentos com o mesmo bloco e número.
- Não podem ser removidos: apartamento com moradores, morador com reservas, funcionário com visitas registradas e área comum com reservas.

---

## Arquitetura

O sistema é dividido em camadas, para que cada parte seja desenvolvida e testada separadamente. As telas não executam SQL diretamente: passam pelos serviços e repositórios.

```
┌────────────────────────────────────────┐
│  Interface (Qt Widgets / terminal)     │   telas e interação com o usuário
├────────────────────────────────────────┤
│  Serviços                              │   regras de negócio
├────────────────────────────────────────┤
│  Repositórios (DAO)                    │   CRUD, isolando o SQL
├────────────────────────────────────────┤
│  Banco de dados (SQLite)               │   persistência
└────────────────────────────────────────┘
      Modelo (entidades) usado por todas as camadas
```

### Hierarquia de classes

```
Pessoa (abstrata)
├── Morador        (tipo de ocupação: proprietário, inquilino, dependente)
├── Visitante
└── Funcionario    (cargo: porteiro, zelador, faxineiro, administrador)

AreaComum (abstrata)
├── SalaoFestas
├── Piscina
└── Churrasqueira

Sem herança: Apartamento, Reserva, Visita
```

### Design patterns

- **Singleton:** conexão única com o banco (`Database::instancia()`).
- **Repository / DAO:** acesso a dados isolado na interface `IRepositorio<T>` e suas implementações.
- **Factory:** `FabricaPessoa` e `FabricaAreaComum` criam o objeto certo a partir do tipo salvo no banco.

Mais detalhes em [`docs/relatorio-arquitetura.md`](docs/relatorio-arquitetura.md).

---

## Conceitos de POO aplicados

| Conceito | Onde aparece no código |
|---|---|
| **Classes e objetos** | Todas as entidades do domínio (`Morador`, `Reserva`, `Apartamento`, `Visita`...) |
| **Herança** | Hierarquias de `Pessoa` e `AreaComum` |
| **Polimorfismo** | Cada área comum calcula a taxa e valida a reserva à sua maneira (`virtual` / `override`); cada pessoa informa o próprio `tipo()` |
| **Classes abstratas** | `Pessoa`, `AreaComum` e a interface `IRepositorio<T>` |
| **Encapsulamento e modificadores de acesso** | Atributos privados/protegidos, acesso por getters e setters com validação (CPF, datas, horários, capacidade) |
| **Ponteiros e referências** | `unique_ptr` em objetos criados pelas fábricas e repositórios, `shared_ptr` nas dependências dos serviços de áreas e reservas, `const&` em parâmetros e ponteiro não proprietário (`Morador*`) em `Apartamento` |
| **Composição e associação** | Associações por identificador: `Morador` guarda o `apartamentoId`; `Reserva` liga morador e área comum; `Visita` liga visitante, apartamento e porteiro. `Apartamento` também mantém uma lista de ponteiros não proprietários para seus moradores, mas hoje as regras de remoção são garantidas pelas chaves estrangeiras do banco |
| **Tratamento de erros** | Hierarquia de exceções própria (`ErroValidacao`, `ErroRegraNegocio`, `ErroBanco`) |

---

## Banco de dados

Utilizamos **SQLite**: um único arquivo `.db`, sem servidor para configurar, o que permite a qualquer pessoa clonar e executar o projeto.

Tabelas: `apartamento`, `pessoa`, `morador`, `funcionario`, `area_comum`, `reserva` e `visita`. Visitantes ficam na tabela `pessoa`, e cada visita é um registro separado, para guardar o histórico.

- Script de criação: [`sql/schema.sql`](sql/schema.sql)
- Dados fictícios para demonstração: [`sql/seed.sql`](sql/seed.sql)
- Diagrama entidade-relacionamento: [`docs/diagrama-er.png`](docs/diagrama-er.png) (explicação em [`docs/modelo-dados.md`](docs/modelo-dados.md))

---

## Estrutura do repositório

```
.
├── CMakeLists.txt
├── README.md
├── docs/                  # relatório (PDF), diagramas, site github.io, guias, testes e roteiro do vídeo
├── sql/
│   ├── schema.sql         # criação das tabelas
│   └── seed.sql           # dados fictícios de demonstração
├── include/               # cabeçalhos (.h)
│   ├── modelo/            # entidades
│   ├── repositorio/       # interfaces e repositórios SQLite
│   ├── servico/           # regras de negócio
│   ├── infra/             # banco, fábricas e exceções
│   └── interface/         # telas Qt e menus do terminal
├── src/                   # implementações (.cpp)
│   ├── modelo/
│   ├── repositorio/
│   ├── servico/
│   ├── infra/
│   ├── interface/
│   ├── terminal/          # main do terminal
│   └── main.cpp           # main da interface gráfica
├── tests/                 # testes automáticos (CTest)
└── third_party/sqlite/    # SQLite embutido (amalgamation)
```

---

## Documentação

| Documento | Conteúdo |
|---|---|
| [`docs/relatorio.pdf`](docs/relatorio.pdf) | Relatório completo do projeto |
| [`docs/diagrama-classes.png`](docs/diagrama-classes.png) (e `.pdf`) | Diagrama de classes final |
| [`docs/index.html`](docs/index.html) | Código-fonte da página github.io |
| [`docs/execucao-local.md`](docs/execucao-local.md) | Compilar e executar localmente |
| [`docs/interface-grafica.md`](docs/interface-grafica.md) | Uso e verificação da interface Qt |
| [`docs/relatorio-arquitetura.md`](docs/relatorio-arquitetura.md) | Camadas e padrões de projeto |
| [`docs/modelo-dados.md`](docs/modelo-dados.md) | Modelo entidade-relacionamento |
| [`docs/testes.md`](docs/testes.md) | Cenários de integração e resultados |
| [`docs/roteiro-video.md`](docs/roteiro-video.md) | Roteiro do vídeo de apresentação |
| [`docs/irepositorio.md`](docs/irepositorio.md) e [`docs/fabricas-i06.md`](docs/fabricas-i06.md) | Interface de repositório e fábricas |

---

## Limitações e trabalhos futuros

- A consulta de reservas por **área e data** existe no serviço (`ReservaService::listarPorArea`) e nos testes, mas ainda não aparece no terminal nem na interface gráfica.
- A edição de área comum não altera o tipo nem o horário de funcionamento, e o terminal não edita nem remove áreas.
- O padrão **Observer** (telas atualizadas automaticamente por signals e slots) não foi implementado: as telas se atualizam ao trocar de aba.
- Ficaram de fora as tarefas extras do planejamento: avisos importantes, subclasses `Porteiro` e `Zelador`, relatórios extras, testes com doctest e banco na nuvem (Supabase).
- Fora do escopo: pagamento real, login e senha, acesso simultâneo e versão web ou mobile. O porteiro é escolhido no formulário, sem autenticação.

## Fluxo de trabalho em equipe

- Branch principal: `main`, sem commits diretos.
- Cada funcionalidade em uma branch própria: `feature/nome-da-funcionalidade`.
- Integração por **Pull Request**, com revisão de pelo menos uma outra pessoa.
- Mensagens de commit curtas e objetivas, em português.

## Referências

- Kirch-Prinz, U.; Prinz, P. _A Complete Guide to Programming in C++_. 2002.
- [Documentação do Qt](https://doc.qt.io/)
- [Documentação do SQLite](https://www.sqlite.org/docs.html)
- [CMake](https://cmake.org/documentation/)
- [cppreference](https://en.cppreference.com/)

## Licença

Projeto acadêmico desenvolvido para fins educacionais.