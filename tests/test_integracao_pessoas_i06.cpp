// teste de integracao do RepositorioPessoa: grava e le morador, visitante e funcionario (a fabrica devolve a subclasse certa)
#include "infra/Database.h"
#include "modelo/Apartamento.h"
#include "modelo/Funcionario.h"
#include "modelo/Morador.h"
#include "modelo/Visitante.h"
#include "repositorio/RepositorioApartamento.h"
#include "repositorio/RepositorioPessoa.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace {

// joga excecao se a condicao for falsa, e assim que o teste falha
void exigir(bool condicao, const char* mensagem) {
    if (!condicao) {
        throw std::runtime_error(mensagem);
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::invalid_argument("Informe o caminho de sql/schema.sql");
    }

    // banco em memoria: o teste nao altera o banco usado pela aplicacao
    Database::instancia(":memory:", argv[1]);
    RepositorioApartamento apartamentos;
    RepositorioPessoa pessoas;

    const int apartamentoId = apartamentos.inserir(Apartamento("A", "101", 1));
    exigir(apartamentoId > 0, "Apartamento nao foi inserido");

    Morador morador("Ana Souza", "52998224725", "81999999999",
                    apartamentoId, TipoOcupacao::Proprietario, "2026-09-01");
    const int moradorId = pessoas.inserir(morador);
    auto pessoaLida = pessoas.buscarPorId(moradorId);
    const auto* moradorLido = dynamic_cast<const Morador*>(pessoaLida.get());
    exigir(moradorLido != nullptr, "A fabrica nao reconstruiu um Morador");
    exigir(moradorLido->apartamentoId() == apartamentoId &&
               moradorLido->cpf() == morador.cpf(),
           "Dados do Morador mudaram ao ler o SQLite");

    Visitante visitante("Bruno Lima", "11144477735", "81988888888");
    const int visitanteId = pessoas.inserir(visitante);
    auto visitanteLido = pessoas.buscarPorId(visitanteId);
    exigir(dynamic_cast<const Visitante*>(visitanteLido.get()) != nullptr,
           "A fabrica nao reconstruiu um Visitante");

    Funcionario funcionario("Carla Dias", "12345678909", "81977777777",
                            Cargo::Porteiro, "manha", "2026-09-01");
    const int funcionarioId = pessoas.inserir(funcionario);
    auto funcionarioLido = pessoas.buscarPorId(funcionarioId);
    exigir(dynamic_cast<const Funcionario*>(funcionarioLido.get()) != nullptr,
           "A fabrica nao reconstruiu um Funcionario");
    exigir(pessoas.listar().size() == 3, "Listagem de pessoas incompleta");

    exigir(pessoas.remover(visitanteId), "Visitante nao foi removido");
    exigir(pessoas.remover(funcionarioId), "Funcionario nao foi removido");
    exigir(pessoas.remover(moradorId), "Morador nao foi removido");
    exigir(apartamentos.remover(apartamentoId), "Apartamento nao foi removido");
}
