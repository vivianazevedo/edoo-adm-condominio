# Sistema de Gestão de Condomínio

Sistema de informação em **C++ orientado a objetos** para administrar um condomínio: moradores, apartamentos, visitantes, funcionários, reservas de áreas comuns e avisos importantes.

> Projeto prático da disciplina **CIN0135 – Estruturas de Dados Orientadas a Objetos**
> Centro de Informática (CIn) – UFPE · Semestre 2026.2 · Prof. Francisco Paulo Magalhães Simões

- **Página do projeto:** _[link do github.io]_
- **Vídeo de apresentação:** _[link do YouTube]_
- **Relatório:** [`docs/relatorio.pdf`](docs/relatorio.pdf)

---

## Objetivo

Oferecer uma primeira experiência prática com **Programação Orientada a Objetos em C++**, construindo um sistema com **CRUD conectado a um banco de dados**, aplicando classes, herança, polimorfismo, encapsulamento, ponteiros/referências e design patterns.

## Funcionalidades

- **Moradores e apartamentos:** cadastro, consulta, atualização e remoção
- **Visitantes:** registro de entrada e saída, com histórico de visitas por apartamento
- **Funcionários:** cadastro de porteiros e zeladores, com cargo e turno
- **Áreas comuns:** salão de festas, piscina e churrasqueira, cada uma com suas regras
- **Reservas:** criar, consultar e cancelar, com validação de conflito de horário, capacidade e horário de funcionamento
- **Avisos:** publicação de avisos gerais, urgentes e de manutenção, com data de expiração

## Arquitetura

O sistema é organizado em camadas, para que cada parte possa ser desenvolvida e testada separadamente:

```
┌────────────────────────────┐
│   Interface (Qt Widgets)   │   telas e interação com o usuário
├────────────────────────────┤
│   Serviços                 │   regras de negócio
├────────────────────────────┤
│   Repositórios (DAO)       │   CRUD, isolando o SQL
├────────────────────────────┤
│   Banco de dados           │   SQLite
└────────────────────────────┘
        Modelo (entidades) usado por todas as camadas
```

### Hierarquia de classes

```
Pessoa (abstrata)
├── Morador
├── Visitante
└── Funcionario
    ├── Porteiro
    └── Zelador

AreaComum (abstrata)
├── SalaoFestas
├── Piscina
└── Churrasqueira

Aviso
├── AvisoGeral
├── AvisoUrgente
└── AvisoManutencao

Apartamento · Reserva
```

O diagrama de classes completo está em [`docs/diagrama-classes.png`](docs/diagrama-classes.png).

## Conceitos de POO aplicados

| Conceito | Onde aparece no código |
|---|---|
| **Classes e objetos** | Todas as entidades do domínio (`Morador`, `Reserva`, `Apartamento`...) |
| **Herança** | Hierarquias de `Pessoa`, `AreaComum` e `Aviso` |
| **Polimorfismo** | Cada `AreaComum` calcula taxa e valida reservas de forma própria (`virtual` / `override`) |
| **Classes abstratas** | `Pessoa`, `AreaComum` e a interface `IRepositorio` |
| **Encapsulamento e modificadores de acesso** | Atributos `private`/`protected`, acesso por getters e setters com validação |
| **Ponteiros e referências** | `unique_ptr` em coleções polimórficas, `const&` em parâmetros, ponteiros em `Reserva` |
| **Composição e associação** | `Apartamento` ↔ `Morador`; `Reserva` ↔ `Morador` + `AreaComum` |

### Design patterns

- **Singleton:** conexão única com o banco (`Database`)
- **Repository / DAO:** acesso a dados isolado em `IRepositorio` e suas implementações
- **Factory:** criação de `Pessoa` e `AreaComum` a partir do tipo salvo no banco
- **Observer:** atualização automática das telas (signals/slots do Qt)

## Banco de dados

Utilizamos **SQLite**: um único arquivo `.db`, sem servidor para configurar, o que permite a qualquer pessoa clonar e executar o projeto.

Tabelas: `apartamento`, `pessoa`, `morador`, `funcionario`, `visita`, `area_comum`, `reserva`, `aviso`.

O script de criação está em [`sql/schema.sql`](sql/schema.sql).

## Tecnologias

- **Linguagem:** C++17
- **Build:** CMake
- **Interface gráfica:** Qt 6 (Widgets)
- **Banco de dados:** SQLite
- **Versionamento:** Git e GitHub

## Estrutura do repositório

```
.
├── CMakeLists.txt
├── README.md
├── docs/                 # relatório, diagrama de classes, imagens
├── sql/
│   └── schema.sql        # criação das tabelas
├── include/              # arquivos de cabeçalho (.h)
│   ├── modelo/
│   ├── repositorio/
│   ├── infra/
│   ├── servico/
│   └── interface/
├── src/                  # implementações (.cpp)
│   ├── modelo/
│   ├── repositorio/
│   ├── infra/
│   ├── servico/
│   ├── interface/
│   └── main.cpp
└── tests/                # testes do núcleo (terminal)
```

> Ajustem esta árvore conforme a estrutura real do projeto.

## Como compilar e executar

### Pré-requisitos

- Compilador com suporte a C++17 (GCC, Clang ou MSVC)
- [CMake](https://cmake.org/) 3.16 ou superior
- [Qt 6](https://www.qt.io/download) (módulo Widgets)
- SQLite3 (biblioteca de desenvolvimento)

### Passo a passo

```bash
# 1. Clonar o repositório
git clone <URL-DO-REPOSITORIO>
cd <NOME-DA-PASTA>

# 2. Configurar e compilar
cmake -S . -B build
cmake --build build

# 3. Executar
./build/condominio        # Linux / macOS
build\condominio.exe      # Windows
```

Na primeira execução, o banco `condominio.db` é criado automaticamente a partir de `sql/schema.sql`.

> Se o CMake não encontrar o Qt, informe o caminho da instalação:
> `cmake -S . -B build -DCMAKE_PREFIX_PATH=<caminho-do-Qt>`

## Como usar

1. Cadastre os **apartamentos** e os **moradores**
2. Cadastre as **áreas comuns** com capacidade, taxa e horário de funcionamento
3. Registre **visitantes** na portaria
4. Crie **reservas** (o sistema bloqueia conflitos de horário e excesso de convidados)
5. Publique **avisos** para os moradores

_Adicionar capturas de tela em `docs/img/` e referenciá-las aqui._

## Regras de negócio

- Não é possível reservar uma área em horário que conflite com outra reserva
- A reserva respeita a capacidade e o horário de funcionamento de cada área
- O cancelamento só é permitido com antecedência mínima definida por área
- Apenas funcionários com cargo de porteiro registram entrada e saída de visitantes
- Avisos urgentes aparecem em destaque; avisos expirados deixam de ser exibidos


## Fluxo de trabalho em equipe

- Branch principal: `main` (protegida, sem commits diretos)
- Cada funcionalidade em uma branch própria: `feature/nome-da-funcionalidade`
- Integração por **Pull Request** com revisão de pelo menos uma outra pessoa
- Mensagens de commit curtas e objetivas, em português

## Referências

- Kirch-Prinz, U.; Prinz, P. _A Complete Guide to Programming in C++_. 2002.
- [Documentação do Qt](https://doc.qt.io/)
- [Documentação do SQLite](https://www.sqlite.org/docs.html)
- [cppreference](https://en.cppreference.com/)

## Licença

Projeto acadêmico desenvolvido para fins educacionais.
