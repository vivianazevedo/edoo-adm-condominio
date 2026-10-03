# D11 — Roteiro do vídeo de apresentação

**Duração-alvo:** 10 minutos (faixa prevista no plano: 8 a 12 minutos). **Estado:** roteiro para aprovação dos quatro integrantes antes da gravação. Cada pessoa grava e narra sua parte; Clécio edita e Beatriz confere os links. Não mostrar na gravação uma funcionalidade que ainda não esteja funcionando no executável final.

| Tempo | Voz | O que mostrar e dizer |
| --- | --- | --- |
| 0:00–1:00 | Sâmia | Título, equipe, objetivo do condomínio e tecnologias: C++17, SQLite, CMake e Qt 6. Abrir o executável final; confirmar antes da gravação se será a GUI ou o terminal. |
| 1:00–3:00 | Clécio | Mostrar a organização `modelo/`, `servico/`, `repositorio/` e `infra/`. No código, apontar `Database::instancia()` (Singleton e abertura/fechamento seguro), os cinco métodos de `IRepositorio<T>` (Repository) e `FabricaPessoa`/`FabricaAreaComum` (Factory). Abrir o diagrama ER e explicar como a FK liga morador a apartamento e reserva a morador/área. |
| 3:00–5:00 | Vivian | Demonstrar cadastro de apartamento, morador e funcionário; registrar uma visita com um funcionário porteiro e a respectiva saída. Mostrar no código a hierarquia `Pessoa` e a regra do porteiro em `VisitaService`. Explicar por que um zelador não pode registrar entrada. |
| 5:00–7:00 | Beatriz | Demonstrar o cadastro de uma área comum, uma reserva válida e uma tentativa com conflito de horário que seja rejeitada. Mostrar `AreaComum` e uma subclasse, explicando o método virtual/`override` e as regras de reserva no serviço. |
| 7:00–9:00 | Sâmia | Percorrer as telas Qt de apartamentos/moradores, áreas/reservas e visitas/funcionários **somente se estiverem integradas e testadas**. Mostrar uma validação real em diálogo. Se a GUI continuar indisponível, demonstrar esses fluxos no terminal e explicar honestamente o estado da interface. |
| 9:00–10:00 | Todos | Cada integrante resume em uma frase o conceito de POO ou aprendizado de sua parte. Encerrar com repositório, relatório e página do projeto. |

## Checklist de gravação

1. Partir da `main` aprovada, compilar do zero e fazer uma execução de ensaio. Preparar dados fictícios; nunca mostrar dados pessoais reais, senha, token ou janela de credenciais.
2. Testar microfone e captura de tela com dez segundos de amostra por pessoa. Usar zoom de editor/terminal suficiente para leitura no vídeo.
3. Gravar um arquivo por bloco, com nome e duração identificáveis. Salvar as falas de cada integrante separadas para facilitar correção de áudio sem regravar tudo.
4. Conferir que as ações exibidas realmente alteram o SQLite e que os erros apresentados são os do programa, não mensagens montadas na edição.
5. Após a aprovação do grupo, editar para 8–12 minutos, publicar no YouTube como público ou não listado conforme decisão do grupo e conferir o link em janela anônima. Só então adicionar o URL definitivo ao README e à página github.io.
