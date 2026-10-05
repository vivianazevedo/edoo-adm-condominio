# D05 — Arquitetura em camadas e padrões de projeto

Esta seção foi escrita para integração no relatório final pela equipe. Ela descreve o código efetivamente presente no repositório; a demonstração da interface Qt depende da conclusão das telas.

## Organização do sistema

O projeto é implementado em C++17 e usa SQLite para persistência. O CMake organiza o núcleo em `condominio_core`, reutilizado pelos executáveis de terminal e, quando Qt 6 Widgets está disponível, da interface gráfica. A divisão do código evita que a interface contenha comandos SQL ou decisões de negócio:

1. `modelo/`: entidades como `Pessoa`, `Morador`, `Funcionario`, `AreaComum`, `Reserva` e `Visita` representam os dados e comportamentos próprios do domínio.
2. `servico/`: coordena os casos de uso e aplica regras, por exemplo a restrição de que somente um porteiro registre visitas e a rejeição de reservas conflitantes.
3. `repositorio/`: executa operações de persistência e converte linhas do SQLite em objetos C++.
4. `infra/`: oferece a conexão com o banco, as exceções da aplicação e as fábricas de objetos polimórficos.
5. `interface/` e `terminal/`: apresentam os dados e encaminham ações do usuário aos serviços. A implementação do terminal já pode ser exercitada; as telas Qt ainda precisam ser integradas aos serviços e verificadas em ambiente com Qt 6.

O fluxo de uma operação é **interface → serviço → repositório → SQLite**. Na leitura, o repositório consulta o banco e devolve objetos do modelo ao serviço, que os entrega à interface. `sql/schema.sql` define sete tabelas: `apartamento`, `pessoa`, `morador`, `funcionario`, `area_comum`, `reserva` e `visita`. A conexão ativa `PRAGMA foreign_keys = ON`, para que as relações declaradas no esquema sejam respeitadas. O diagrama ER e a descrição das chaves estão em `docs/modelo-dados.md`.

## Singleton e gerenciamento de recursos

`Database::instancia()` retorna uma única instância por processo. A primeira chamada escolhe o caminho do arquivo `.db` e o arquivo de esquema; chamadas seguintes reutilizam a mesma conexão. O construtor abre o SQLite, ativa as chaves estrangeiras e cria as tabelas quando o banco ainda está vazio. O destrutor fecha a conexão. A cópia e a movimentação são desabilitadas, evitando múltiplos donos do mesmo recurso.

Esse é o padrão **Singleton** combinado a **RAII** (*Resource Acquisition Is Initialization*): abrir e fechar a conexão acompanham a vida do objeto, inclusive quando uma exceção interrompe a inicialização. A escolha simplifica um aplicativo local e monousuário, mas implica uma conexão global; para concorrência ou múltiplos bancos simultâneos seria necessário rever essa decisão.

## Repository

`IRepositorio<T>` define um contrato genérico de CRUD: `inserir`, `buscarPorId`, `listar`, `atualizar` e `remover`. As implementações concretas, como `RepositorioPessoa` e `RepositorioAreaComum`, escondem o SQL dos serviços. `buscarPorId` retorna `std::unique_ptr<T>` e `listar` retorna um vetor desses ponteiros. Assim, o dono de cada objeto fica explícito e é possível devolver subclasses por um ponteiro para a classe base abstrata.

Em `RepositorioPessoa`, a gravação dos dados comuns de `pessoa` e dos dados específicos de `morador` ou `funcionario` é protegida por uma transação: ou todas as inserções são confirmadas, ou são desfeitas. Os comandos preparados usam parâmetros para os valores, mantendo a montagem do SQL separada dos dados do usuário. Os serviços dependem dos contratos de repositório e podem ser testados sobre um SQLite temporário em memória.

## Factory

O banco salva um discriminador de tipo, mas não um objeto C++. `FabricaPorTipo<Base>` associa esse texto a funções que constroem subclasses e rejeita um tipo não registrado. `FabricaPessoa` registra `morador`, `visitante` e `funcionario`; `FabricaAreaComum` registra `SalaoFestas`, `Piscina` e `Churrasqueira`. Ao ler uma linha, o repositório usa a fábrica para devolver, por exemplo, um `std::unique_ptr<Pessoa>` que contém de fato um `Morador`.

O padrão **Factory** concentra a escolha da subclasse, preserva o polimorfismo depois de uma leitura do banco e evita espalhar essa decisão pelas telas. Os testes verificam a reconstrução de `Morador` e `Piscina` a partir de dados persistidos.

## Polimorfismo e Regras de Negócio (Áreas Comuns e Reservas)

O módulo de Áreas Comuns e Reservas exemplifica a aplicação de **polimorfismo em tempo de execução** para gerir diferentes regras de tarifação e capacidade sem acoplar a camada de serviços às classes concretas.

### Hierarquia e Sobrescrita de Métodos

A classe abstrata `AreaComum` declara dois métodos virtuais puros que são redefinidos (`override`) pelas subclasses:

* `calcularTaxa(int numConvidados)`:
  * `SalaoFestas`: Calcula a taxa base acrescida de um valor variável por convidado.
  * `Piscina`: Isento de taxa (retorna R$ 0,00).
  * `Churrasqueira`: Aplica uma taxa fixa, independentemente do número de convidados.

* `validarReserva(int numConvidados, std::string horaInicio, std::string horaFim)`:
  * `SalaoFestas`: Valida se o número de convidados respeita o limite do salão (até 80 pessoas).
  * `Piscina`: Garante a restrição de no máximo 4 convidados por reserva.
  * `Churrasqueira`: Valida o limite de ocupação do espaço (até 20 pessoas).

### Regras de Negócio do `ReservaService`

A camada de serviço (`ReservaService`) orquestra o fluxo de reserva executando três verificações essenciais antes da persistência no SQLite:

1. **Validação Polimórfica:** Invocação do método `validarReserva()` da subclasse específica da área para validar regras de capacidade.
2. **Prevenção de Sobreposição de Horários (US06):** Consulta do `RepositorioReserva` para assegurar que não existem reservas ativas na mesma área e data cujos intervalos de tempo (`horaInicio` e `horaFim`) se sobreponham.
3. **Cálculo Automático:** Registo do custo total da reserva invocando `calcularTaxa()` diretamente sobre o objeto polimórfico instanciado pela fábrica.

## Verificação e limites

Os dez cenários de integração D02 e os comandos para reproduzi-los estão em `docs/testes.md`. Em 02/10/2026, o núcleo compilou com MSVC 19.51 e os testes então registrados passaram; a compilação da interface Qt não pôde ser confirmada na máquina usada porque Qt 6 Widgets não estava instalado. A equipe deve repetir o build com Qt e registrar um segundo computador antes de afirmar que o critério completo de G05 foi atingido.
