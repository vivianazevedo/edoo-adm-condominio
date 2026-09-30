#include "servico/ApartamentoService.h"
#include <string>
#include "infra/ErroCondominio.h"

namespace {

// procura outro apartamento com o mesmo bloco e numero
// ignorarId e o id do proprio apartamento na edicao (0 no cadastro)
bool existeDuplicado(IRepositorio<Apartamento>& repo, const Apartamento& novo, int ignorarId) {
    for (const auto& existente : repo.listar()) {
        if (existente->getId() != ignorarId &&
            existente->getBloco() == novo.getBloco() &&
            existente->getNumero() == novo.getNumero()) {
            return true;
        }
    }
    return false;
}

}  // namespace

ApartamentoService::ApartamentoService(IRepositorio<Apartamento>& repo) : repo_(repo) {}

int ApartamentoService::cadastrar(const string& bloco, const string& numero, int andar) {
    // o construtor valida os campos e joga ErroValidacao se estiver errado
    Apartamento novo(bloco, numero, andar);

    if (existeDuplicado(repo_, novo, 0)) {
        throw ErroRegraNegocio("ja existe o apartamento " + novo.getBloco() + "-" + novo.getNumero());
    }
    return repo_.inserir(novo);
}

vector<unique_ptr<Apartamento>> ApartamentoService::listar() {
    return repo_.listar();
}

unique_ptr<Apartamento> ApartamentoService::buscar(int id) {
    return repo_.buscarPorId(id);
}

void ApartamentoService::editar(int id, const string& bloco, const string& numero, int andar) {
    if (repo_.buscarPorId(id) == nullptr) {
        throw ErroRegraNegocio("apartamento " + to_string(id) + " nao encontrado");
    }

    Apartamento editado(bloco, numero, andar, id);

    if (existeDuplicado(repo_, editado, id)) {
        throw ErroRegraNegocio("ja existe o apartamento " + editado.getBloco() + "-" + editado.getNumero());
    }
    if (!repo_.atualizar(editado)) {
        throw ErroRegraNegocio("apartamento " + to_string(id) + " nao encontrado");
    }
}

void ApartamentoService::remover(int id) {
    if (repo_.buscarPorId(id) == nullptr) {
        throw ErroRegraNegocio("apartamento " + to_string(id) + " nao encontrado");
    }

    // RN08: o banco recusa apagar apartamento que ainda tem morador (chave estrangeira)
    // se o erro for esse, avisa com uma mensagem de regra de negocio
    try {
        repo_.remover(id);
    } catch (const ErroBanco& e) {
        string mensagem = e.what();
        if (mensagem.find("FOREIGN KEY") != string::npos) {
            throw ErroRegraNegocio("apartamento com moradores nao pode ser removido");
        }
        throw;
    }
}