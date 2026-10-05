#include "interface/MenuMoradores.h"
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include "infra/ErroCondominio.h"
#include "modelo/Apartamento.h"
#include "modelo/Funcionario.h"
#include "modelo/Morador.h"

using namespace std;  // permitido em .cpp (so e proibido nos .h)

namespace {

// le uma linha inteira (aceita espacos), se a entrada acabar devolve vazio
string lerLinha(const string& pergunta) {
    cout << pergunta;
    string linha;
    getline(cin, linha);
    return linha;
}

// le um numero inteiro, repete a pergunta se digitarem outra coisa
// se a entrada acabar (ctrl+d) devolve 0 para o menu voltar
int lerInteiro(const string& pergunta) {
    while (true) {
        string linha = lerLinha(pergunta);
        if (!cin) {
            return 0;
        }
        try {
            size_t usados = 0;
            int valor = stoi(linha, &usados);
            if (usados == linha.size()) {
                return valor;
            }
        } catch (const exception&) {
            // cai na mensagem de baixo
        }
        cout << "Numero invalido, tente de novo.\n";
    }
}

// roda a acao e mostra o erro (se der) sem derrubar o menu
void executar(const function<void()>& acao) {
    try {
        acao();
    } catch (const ErroValidacao& e) {
        cout << "Dados invalidos: " << e.what() << "\n";
    } catch (const ErroRegraNegocio& e) {
        cout << "Regra de negocio: " << e.what() << "\n";
    } catch (const ErroBanco& e) {
        cout << "Erro no banco: " << e.what() << "\n";
    }
}

// converte o enum em texto pra mostrar no menu
string ocupacaoParaTexto(TipoOcupacao ocupacao) {
    if (ocupacao == TipoOcupacao::Proprietario) return "Proprietario";
    if (ocupacao == TipoOcupacao::Inquilino) return "Inquilino";
    return "Dependente";
}

// converte o enum em texto pra mostrar no menu
string cargoParaTexto(Cargo cargo) {
    if (cargo == Cargo::Porteiro) return "Porteiro";
    if (cargo == Cargo::Zelador) return "Zelador";
    if (cargo == Cargo::Faxineiro) return "Faxineiro";
    return "Administrador";
}

// pergunta a ocupacao ate a pessoa escolher uma opcao valida
TipoOcupacao lerOcupacao() {
    while (true) {
        cout << "Ocupacao: 1 Proprietario | 2 Inquilino | 3 Dependente\n";
        int opcao = lerInteiro("Escolha: ");
        if (!cin) {
            throw ErroValidacao("entrada encerrada");
        }
        if (opcao == 1) return TipoOcupacao::Proprietario;
        if (opcao == 2) return TipoOcupacao::Inquilino;
        if (opcao == 3) return TipoOcupacao::Dependente;
        cout << "Opcao invalida.\n";
    }
}

// pergunta o cargo ate a pessoa escolher uma opcao valida
Cargo lerCargo() {
    while (true) {
        cout << "Cargo: 1 Porteiro | 2 Zelador | 3 Faxineiro | 4 Administrador\n";
        int opcao = lerInteiro("Escolha: ");
        if (!cin) {
            throw ErroValidacao("entrada encerrada");
        }
        if (opcao == 1) return Cargo::Porteiro;
        if (opcao == 2) return Cargo::Zelador;
        if (opcao == 3) return Cargo::Faxineiro;
        if (opcao == 4) return Cargo::Administrador;
        cout << "Opcao invalida.\n";
    }
}

// imprime uma linha com os dados do morador
// o dynamic_cast pega os campos que so o Morador tem
void imprimirMorador(const Pessoa& pessoa) {
    cout << "ID " << pessoa.id() << " | " << pessoa.nome()
         << " | CPF " << pessoa.cpf() << " | Tel " << pessoa.telefone();
    const Morador* morador = dynamic_cast<const Morador*>(&pessoa);
    if (morador != nullptr) {
        cout << " | Apto " << morador->apartamentoId()
             << " | " << ocupacaoParaTexto(morador->tipoOcupacao())
             << " | Entrada " << morador->dataEntrada();
    }
    cout << "\n";
}

// imprime uma linha com os dados do funcionario (dynamic_cast, igual ao do morador)
void imprimirFuncionario(const Pessoa& pessoa) {
    cout << "ID " << pessoa.id() << " | " << pessoa.nome()
         << " | CPF " << pessoa.cpf() << " | Tel " << pessoa.telefone();
    const Funcionario* funcionario = dynamic_cast<const Funcionario*>(&pessoa);
    if (funcionario != nullptr) {
        cout << " | " << cargoParaTexto(funcionario->cargo())
             << " | Turno " << funcionario->turno()
             << " | Admissao " << funcionario->dataAdmissao();
    }
    cout << "\n";
}

}  // namespace

// guarda as referencias dos services
MenuMoradores::MenuMoradores(ApartamentoService& apartamentoService,
                             MoradorService& moradorService,
                             FuncionarioService& funcionarioService)
    : apartamentoService_(apartamentoService),
      moradorService_(moradorService),
      funcionarioService_(funcionarioService) {}

// menu do modulo: repete ate a pessoa escolher 0 (voltar)
void MenuMoradores::exibirMenu() {
    int opcao = -1;
    while (opcao != 0) {
        cout << "\n========================================\n";
        cout << "  Apartamentos, Moradores e Funcionarios\n";
        cout << "========================================\n";
        cout << "1. Apartamentos\n";
        cout << "2. Moradores\n";
        cout << "3. Funcionarios\n";
        cout << "0. Voltar ao menu principal\n";
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1: menuApartamentos(); break;
            case 2: menuMoradores(); break;
            case 3: menuFuncionarios(); break;
            case 0: break;
            default: cout << "Opcao invalida!\n"; break;
        }
    }
}

// submenu de apartamentos
// cada opcao roda dentro do executar(), assim o erro aparece na tela e o programa nao fecha
void MenuMoradores::menuApartamentos() {
    int opcao = -1;
    while (opcao != 0) {
        cout << "\n--- Apartamentos ---\n";
        cout << "1. Cadastrar\n";
        cout << "2. Listar\n";
        cout << "3. Editar\n";
        cout << "4. Remover\n";
        cout << "0. Voltar\n";
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1: executar([this] { cadastrarApartamento(); }); break;
            case 2: executar([this] { listarApartamentos(); }); break;
            case 3: executar([this] { editarApartamento(); }); break;
            case 4: executar([this] { removerApartamento(); }); break;
            case 0: break;
            default: cout << "Opcao invalida!\n"; break;
        }
    }
}

// submenu de moradores (mesma ideia do de apartamentos)
void MenuMoradores::menuMoradores() {
    int opcao = -1;
    while (opcao != 0) {
        cout << "\n--- Moradores ---\n";
        cout << "1. Cadastrar\n";
        cout << "2. Listar todos\n";
        cout << "3. Listar por apartamento\n";
        cout << "4. Editar\n";
        cout << "5. Remover\n";
        cout << "0. Voltar\n";
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1: executar([this] { cadastrarMorador(); }); break;
            case 2: executar([this] { listarMoradores(); }); break;
            case 3: executar([this] { listarMoradoresPorApartamento(); }); break;
            case 4: executar([this] { editarMorador(); }); break;
            case 5: executar([this] { removerMorador(); }); break;
            case 0: break;
            default: cout << "Opcao invalida!\n"; break;
        }
    }
}

// submenu de funcionarios (mesma ideia)
void MenuMoradores::menuFuncionarios() {
    int opcao = -1;
    while (opcao != 0) {
        cout << "\n--- Funcionarios ---\n";
        cout << "1. Cadastrar\n";
        cout << "2. Listar\n";
        cout << "3. Editar\n";
        cout << "4. Remover\n";
        cout << "0. Voltar\n";
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1: executar([this] { cadastrarFuncionario(); }); break;
            case 2: executar([this] { listarFuncionarios(); }); break;
            case 3: executar([this] { editarFuncionario(); }); break;
            case 4: executar([this] { removerFuncionario(); }); break;
            case 0: break;
            default: cout << "Opcao invalida!\n"; break;
        }
    }
}

// ---------- apartamentos ----------

// pede os dados e chama o service pra cadastrar
void MenuMoradores::cadastrarApartamento() {
    cout << "\n--- Novo apartamento ---\n";
    string bloco = lerLinha("Bloco: ");
    string numero = lerLinha("Numero: ");
    int andar = lerInteiro("Andar: ");

    int id = apartamentoService_.cadastrar(bloco, numero, andar);
    cout << "Apartamento cadastrado! ID: " << id << "\n";
}

// lista todos (ou avisa que nao tem nenhum)
void MenuMoradores::listarApartamentos() {
    auto apartamentos = apartamentoService_.listar();
    cout << "\n--- Apartamentos cadastrados ---\n";
    if (apartamentos.empty()) {
        cout << "Nenhum apartamento cadastrado.\n";
        return;
    }
    for (const auto& apartamento : apartamentos) {
        cout << "ID " << apartamento->getId() << " | " << apartamento->descricao() << "\n";
    }
}

// pede o id e os dados novos e chama o service pra editar
void MenuMoradores::editarApartamento() {
    cout << "\n--- Editar apartamento ---\n";
    int id = lerInteiro("ID do apartamento: ");
    string bloco = lerLinha("Novo bloco: ");
    string numero = lerLinha("Novo numero: ");
    int andar = lerInteiro("Novo andar: ");

    apartamentoService_.editar(id, bloco, numero, andar);
    cout << "Apartamento atualizado!\n";
}

// pede o id e chama o service pra remover
void MenuMoradores::removerApartamento() {
    cout << "\n--- Remover apartamento ---\n";
    int id = lerInteiro("ID do apartamento: ");

    apartamentoService_.remover(id);
    cout << "Apartamento removido!\n";
}

// ---------- moradores ----------

// pede os dados e chama o service pra cadastrar
void MenuMoradores::cadastrarMorador() {
    cout << "\n--- Novo morador ---\n";
    string nome = lerLinha("Nome: ");
    string cpf = lerLinha("CPF: ");
    string telefone = lerLinha("Telefone: ");
    int apartamentoId = lerInteiro("ID do apartamento: ");
    TipoOcupacao ocupacao = lerOcupacao();
    string dataEntrada = lerLinha("Data de entrada (AAAA-MM-DD): ");

    int id = moradorService_.cadastrar(nome, cpf, telefone, apartamentoId, ocupacao, dataEntrada);
    cout << "Morador cadastrado! ID: " << id << "\n";
}

// lista todos (ou avisa que nao tem nenhum)
void MenuMoradores::listarMoradores() {
    auto moradores = moradorService_.listar();
    cout << "\n--- Moradores cadastrados ---\n";
    if (moradores.empty()) {
        cout << "Nenhum morador cadastrado.\n";
        return;
    }
    for (const auto& morador : moradores) {
        imprimirMorador(*morador);
    }
}

// pede o id do apartamento e lista so os moradores dele
void MenuMoradores::listarMoradoresPorApartamento() {
    cout << "\n--- Moradores por apartamento ---\n";
    int apartamentoId = lerInteiro("ID do apartamento: ");

    auto moradores = moradorService_.listarPorApartamento(apartamentoId);
    if (moradores.empty()) {
        cout << "Nenhum morador nesse apartamento.\n";
        return;
    }
    for (const auto& morador : moradores) {
        imprimirMorador(*morador);
    }
}

// pede o id e os dados novos e chama o service pra editar
void MenuMoradores::editarMorador() {
    cout << "\n--- Editar morador ---\n";
    int id = lerInteiro("ID do morador: ");
    string nome = lerLinha("Novo nome: ");
    string cpf = lerLinha("Novo CPF: ");
    string telefone = lerLinha("Novo telefone: ");
    int apartamentoId = lerInteiro("Novo ID do apartamento: ");
    TipoOcupacao ocupacao = lerOcupacao();
    string dataEntrada = lerLinha("Nova data de entrada (AAAA-MM-DD): ");

    moradorService_.editar(id, nome, cpf, telefone, apartamentoId, ocupacao, dataEntrada);
    cout << "Morador atualizado!\n";
}

// pede o id e chama o service pra remover
void MenuMoradores::removerMorador() {
    cout << "\n--- Remover morador ---\n";
    int id = lerInteiro("ID do morador: ");

    moradorService_.remover(id);
    cout << "Morador removido!\n";
}

// ---------- funcionarios ----------

// pede os dados e chama o service pra cadastrar
void MenuMoradores::cadastrarFuncionario() {
    cout << "\n--- Novo funcionario ---\n";
    string nome = lerLinha("Nome: ");
    string cpf = lerLinha("CPF: ");
    string telefone = lerLinha("Telefone: ");
    Cargo cargo = lerCargo();
    string turno = lerLinha("Turno (manha, tarde ou noite): ");
    string dataAdmissao = lerLinha("Data de admissao (AAAA-MM-DD): ");

    int id = funcionarioService_.cadastrar(nome, cpf, telefone, cargo, turno, dataAdmissao);
    cout << "Funcionario cadastrado! ID: " << id << "\n";
}

// lista todos (ou avisa que nao tem nenhum)
void MenuMoradores::listarFuncionarios() {
    auto funcionarios = funcionarioService_.listar();
    cout << "\n--- Funcionarios cadastrados ---\n";
    if (funcionarios.empty()) {
        cout << "Nenhum funcionario cadastrado.\n";
        return;
    }
    for (const auto& funcionario : funcionarios) {
        imprimirFuncionario(*funcionario);
    }
}

// pede o id e os dados novos e chama o service pra editar
void MenuMoradores::editarFuncionario() {
    cout << "\n--- Editar funcionario ---\n";
    int id = lerInteiro("ID do funcionario: ");
    string nome = lerLinha("Novo nome: ");
    string cpf = lerLinha("Novo CPF: ");
    string telefone = lerLinha("Novo telefone: ");
    Cargo cargo = lerCargo();
    string turno = lerLinha("Novo turno: ");
    string dataAdmissao = lerLinha("Nova data de admissao (AAAA-MM-DD): ");

    funcionarioService_.editar(id, nome, cpf, telefone, cargo, turno, dataAdmissao);
    cout << "Funcionario atualizado!\n";
}

// pede o id e chama o service pra remover
void MenuMoradores::removerFuncionario() {
    cout << "\n--- Remover funcionario ---\n";
    int id = lerInteiro("ID do funcionario: ");

    funcionarioService_.remover(id);
    cout << "Funcionario removido!\n";
}