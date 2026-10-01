#include "infra/Database.h"
#include "infra/ErroCondominio.h"
#include "repositorio/RepositorioApartamento.h"
#include "repositorio/RepositorioPessoa.h"
#include "servico/ApartamentoService.h"
#include "servico/FuncionarioService.h"
#include "servico/MoradorService.h"

#include <stdexcept>

namespace {

void exigir(bool condicao, const char* mensagem) {
    if (!condicao) throw std::runtime_error(mensagem);
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) throw std::invalid_argument("Informe o caminho de sql/schema.sql");

    // O banco existe apenas durante este teste.
    Database::instancia(":memory:", argv[1]);
    RepositorioApartamento repoApartamentos;
    RepositorioPessoa repoPessoas;
    ApartamentoService apartamentos(repoApartamentos);
    MoradorService moradores(repoPessoas, repoApartamentos);
    FuncionarioService funcionarios(repoPessoas);

    const int apartamentoId = apartamentos.cadastrar("A", "101", 1);
    const int moradorId = moradores.cadastrar("Ana Souza", "52998224725", "81999999999",
                                             apartamentoId, TipoOcupacao::Proprietario,
                                             "2026-09-01");
    const int funcionarioId = funcionarios.cadastrar("Carla Dias", "12345678909",
                                                     "81977777777", Cargo::Porteiro,
                                                     "manha", "2026-09-01");

    exigir(apartamentos.buscar(apartamentoId) != nullptr, "Apartamento nao encontrado");
    exigir(moradores.listar().size() == 1, "Listagem de moradores incorreta");
    exigir(moradores.listarPorApartamento(apartamentoId).size() == 1,
           "Morador nao vinculado ao apartamento");
    exigir(funcionarios.listar().size() == 1, "Listagem de funcionarios incorreta");

    bool cpfDuplicadoRejeitado = false;
    try {
        funcionarios.cadastrar("Outra Pessoa", "52998224725", "81966666666",
                               Cargo::Porteiro, "tarde", "2026-09-02");
    } catch (const ErroRegraNegocio&) {
        cpfDuplicadoRejeitado = true;
    }
    exigir(cpfDuplicadoRejeitado, "CPF duplicado foi aceito entre tipos de pessoa");

    bool apartamentoOcupadoProtegido = false;
    try {
        apartamentos.remover(apartamentoId);
    } catch (const ErroRegraNegocio&) {
        apartamentoOcupadoProtegido = true;
    }
    exigir(apartamentoOcupadoProtegido, "Apartamento com morador foi removido");

    moradores.remover(moradorId);
    funcionarios.remover(funcionarioId);
    apartamentos.remover(apartamentoId);
    exigir(apartamentos.buscar(apartamentoId) == nullptr, "Apartamento nao foi removido");
}
