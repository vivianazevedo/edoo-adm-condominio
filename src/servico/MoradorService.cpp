#include "servico/MoradorService.h"
#include <string>
#include <utility>
#include "infra/ErroCondominio.h"

using namespace std;  // permitido em .cpp (so e proibido nos .h)

namespace {

// confere se o apartamento existe, senao joga ErroRegraNegocio
void garantirApartamento(IRepositorio<Apartamento>& repo, int apartamentoId) {
    if (repo.buscarPorId(apartamentoId) == nullptr) {
        throw ErroRegraNegocio("apartamento " + to_string(apartamentoId) + " nao encontrado");
    }
}

// RN07: o cpf tem que ser unico entre todas as pessoas (morador, funcionario e visitante)
// ignorarId e o id da propria pessoa na edicao (0 no cadastro)
bool cpfEmUso(IRepositorio<Pessoa>& repo, const string& cpf, int ignorarId) {
    for (const auto& pessoa : repo.listar()) {
        if (pessoa->id() != ignorarId && pessoa->cpf() == cpf) {
            return true;
        }
    }
    return false;
}

// confere se o id e de um morador de verdade (pessoa pode ser funcionario ou visitante)
void garantirMorador(IRepositorio<Pessoa>& repo, int id) {
    unique_ptr<Pessoa> pessoa = repo.buscarPorId(id);
    if (dynamic_cast<Morador*>(pessoa.get()) == nullptr) {
        throw ErroRegraNegocio("morador " + to_string(id) + " nao encontrado");
    }
}

}  // namespace

MoradorService::MoradorService(IRepositorio<Pessoa>& repoPessoa, IRepositorio<Apartamento>& repoApto)
    : repoPessoa_(repoPessoa), repoApto_(repoApto) {}

int MoradorService::cadastrar(const string& nome, const string& cpf, const string& telefone,
                              int apartamentoId, TipoOcupacao tipoOcupacao,
                              const string& dataEntrada) {
    // o construtor valida nome, cpf, telefone e data (ErroValidacao)
    Morador novo(nome, cpf, telefone, apartamentoId, tipoOcupacao, dataEntrada);

    garantirApartamento(repoApto_, apartamentoId);

    if (cpfEmUso(repoPessoa_, novo.cpf(), 0)) {
        throw ErroRegraNegocio("ja existe uma pessoa com o CPF " + novo.cpf());
    }
    return repoPessoa_.inserir(novo);
}

vector<unique_ptr<Pessoa>> MoradorService::listar() {
    vector<unique_ptr<Pessoa>> moradores;
    for (auto& pessoa : repoPessoa_.listar()) {
        if (dynamic_cast<Morador*>(pessoa.get()) != nullptr) {
            moradores.push_back(std::move(pessoa));
        }
    }
    return moradores;
}

vector<unique_ptr<Pessoa>> MoradorService::listarPorApartamento(int apartamentoId) {
    garantirApartamento(repoApto_, apartamentoId);

    vector<unique_ptr<Pessoa>> moradores;
    for (auto& pessoa : repoPessoa_.listar()) {
        const Morador* morador = dynamic_cast<const Morador*>(pessoa.get());
        if (morador != nullptr && morador->apartamentoId() == apartamentoId) {
            moradores.push_back(std::move(pessoa));
        }
    }
    return moradores;
}

void MoradorService::editar(int id, const string& nome, const string& cpf,
                            const string& telefone, int apartamentoId,
                            TipoOcupacao tipoOcupacao, const string& dataEntrada) {
    garantirMorador(repoPessoa_, id);

    Morador editado(nome, cpf, telefone, apartamentoId, tipoOcupacao, dataEntrada, id);

    garantirApartamento(repoApto_, apartamentoId);

    if (cpfEmUso(repoPessoa_, editado.cpf(), id)) {
        throw ErroRegraNegocio("ja existe uma pessoa com o CPF " + editado.cpf());
    }
    if (!repoPessoa_.atualizar(editado)) {
        throw ErroRegraNegocio("morador " + to_string(id) + " nao encontrado");
    }
}

void MoradorService::remover(int id) {
    garantirMorador(repoPessoa_, id);

    // RN08: o banco recusa apagar morador que tem reserva (chave estrangeira)
    // se o erro for esse, avisa com uma mensagem de regra de negocio
    try {
        if (!repoPessoa_.remover(id)) {
            throw ErroRegraNegocio("morador " + to_string(id) + " nao encontrado");
        }
    } catch (const ErroBanco& e) {
        string mensagem = e.what();
        if (mensagem.find("FOREIGN KEY") != string::npos) {
            throw ErroRegraNegocio("morador com reservas nao pode ser removido");
        }
        throw;
    }
}