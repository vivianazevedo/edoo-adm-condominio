#include "servico/FuncionarioService.h"
#include <string>
#include <utility>
#include "infra/ErroCondominio.h"

using namespace std;  // permitido em .cpp (so e proibido nos .h)

namespace {

// rn07: o cpf tem que ser unico entre todas as pessoas (morador, funcionario e visitante)
// ignorarId e o id da propria pessoa na edicao (0 no cadastro)
bool cpfEmUso(IRepositorio<Pessoa>& repo, const string& cpf, int ignorarId) {
    for (const auto& pessoa : repo.listar()) {
        if (pessoa->id() != ignorarId && pessoa->cpf() == cpf) {
            return true;
        }
    }
    return false;
}

// confere se o id e de um funcionario de verdade (pessoa pode ser morador ou visitante)
void garantirFuncionario(IRepositorio<Pessoa>& repo, int id) {
    unique_ptr<Pessoa> pessoa = repo.buscarPorId(id);
    if (dynamic_cast<Funcionario*>(pessoa.get()) == nullptr) {
        throw ErroRegraNegocio("funcionario " + to_string(id) + " nao encontrado");
    }
}

}  // namespace

// guarda a referencia do repositorio
FuncionarioService::FuncionarioService(IRepositorio<Pessoa>& repoPessoa)
    : repoPessoa_(repoPessoa) {}

// valida pelo construtor, confere se o cpf ja existe (RN07) e salva
int FuncionarioService::cadastrar(const string& nome, const string& cpf, const string& telefone,
                                  Cargo cargo, const string& turno, const string& dataAdmissao) {
    // o construtor valida nome, cpf, telefone, turno e data (ErroValidacao)
    Funcionario novo(nome, cpf, telefone, cargo, turno, dataAdmissao);

    if (cpfEmUso(repoPessoa_, novo.cpf(), 0)) {
        throw ErroRegraNegocio("ja existe uma pessoa com o CPF " + novo.cpf());
    }
    return repoPessoa_.inserir(novo);
}

// pega todas as pessoas e fica so com os funcionarios (dynamic_cast)
vector<unique_ptr<Pessoa>> FuncionarioService::listar() {
    vector<unique_ptr<Pessoa>> funcionarios;
    for (auto& pessoa : repoPessoa_.listar()) {
        if (dynamic_cast<Funcionario*>(pessoa.get()) != nullptr) {
            funcionarios.push_back(std::move(pessoa));
        }
    }
    return funcionarios;
}

// confere que e um funcionario, valida os dados novos e ve se o cpf nao e de outra pessoa
void FuncionarioService::editar(int id, const string& nome, const string& cpf,
                                const string& telefone, Cargo cargo, const string& turno,
                                const string& dataAdmissao) {
    garantirFuncionario(repoPessoa_, id);

    Funcionario editado(nome, cpf, telefone, cargo, turno, dataAdmissao, id);

    if (cpfEmUso(repoPessoa_, editado.cpf(), id)) {
        throw ErroRegraNegocio("ja existe uma pessoa com o CPF " + editado.cpf());
    }
    if (!repoPessoa_.atualizar(editado)) {
        throw ErroRegraNegocio("funcionario " + to_string(id) + " nao encontrado");
    }
}

// confere que e um funcionario e tenta remover
void FuncionarioService::remover(int id) {
    garantirFuncionario(repoPessoa_, id);

    // o banco recusa apagar funcionario que registrou visita (chave estrangeira)
    // se o erro for esse, avisa com uma mensagem de regra de negocio
    try {
        if (!repoPessoa_.remover(id)) {
            throw ErroRegraNegocio("funcionario " + to_string(id) + " nao encontrado");
        }
    } catch (const ErroBanco& e) {
        string mensagem = e.what();
        if (mensagem.find("FOREIGN KEY") != string::npos) {
            throw ErroRegraNegocio("funcionario com visitas registradas nao pode ser removido");
        }
        throw;
    }
}