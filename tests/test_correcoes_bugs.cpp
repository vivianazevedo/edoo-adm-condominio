// Testes das correcoes de bugs: datas validas, texto so com espacos, erros de chave
// estrangeira traduzidos para regra de negocio e reserva em data passada.
#include <functional>
#include <iostream>
#include <string>
#include "infra/Database.h"
#include "infra/ErroCondominio.h"
#include "repositorio/RepositorioApartamento.h"
#include "repositorio/RepositorioPessoa.h"
#include "repositorio/RepositorioVisita.h"
#include "servico/ApartamentoService.h"
#include "servico/AreaComumService.h"
#include "servico/FuncionarioService.h"
#include "servico/MoradorService.h"
#include "servico/ReservaService.h"
#include "servico/VisitaService.h"

namespace {

int falhas = 0;

void exigir(bool condicao, const std::string& mensagem) {
    if (!condicao) {
        std::cerr << "FALHOU: " << mensagem << "\n";
        ++falhas;
    }
}

// exige que a acao lance exatamente o tipo de erro T
template <typename T>
void exigirErro(const std::function<void()>& acao, const std::string& mensagem) {
    try {
        acao();
    } catch (const T&) {
        return;
    } catch (const std::exception& e) {
        std::cerr << "FALHOU: " << mensagem << " (tipo errado: " << e.what() << ")\n";
        ++falhas;
        return;
    }
    std::cerr << "FALHOU: " << mensagem << " (nao lancou erro)\n";
    ++falhas;
}

}  // namespace

int main(int argc, char* argv[]) {
    const std::string schema = argc > 1 ? argv[1] : "sql/schema.sql";
    Database::instancia(":memory:", schema);

    RepositorioApartamento repoApto;
    RepositorioPessoa repoPessoa;
    RepositorioVisita repoVisita;
    ApartamentoService aptos(repoApto);
    MoradorService moradores(repoPessoa, repoApto);
    FuncionarioService funcionarios(repoPessoa);
    VisitaService visitas(repoPessoa, repoVisita);
    AreaComumService areas;
    ReservaService reservas;

    const int apto = aptos.cadastrar("A", "101", 1);
    const int morador = moradores.cadastrar("Ana", "52998224725", "81999999999", apto,
                                            TipoOcupacao::Proprietario, "2026-01-01");
    const int porteiro = funcionarios.cadastrar("Beto", "11144477735", "81988888888",
                                                Cargo::Porteiro, "manha", "2026-01-01");
    const int visitante = visitas.cadastrarVisitante("Caio", "39053344705", "81977777777");
    const int area = areas.cadastrar("SalaoFestas", "Salao", 80, 250, "08:00", "23:00");

    // bug 1: datas que nao existem
    exigirErro<ErroValidacao>([&] {
        moradores.cadastrar("Duda", "15350946056", "81922222222", apto,
                            TipoOcupacao::Inquilino, "2026-13-45");
    }, "mes 13 deveria ser recusado");
    exigirErro<ErroValidacao>([&] {
        moradores.cadastrar("Duda", "15350946056", "81922222222", apto,
                            TipoOcupacao::Inquilino, "2026-02-30");
    }, "30 de fevereiro deveria ser recusado");
    exigirErro<ErroValidacao>([&] {
        moradores.cadastrar("Duda", "15350946056", "81922222222", apto,
                            TipoOcupacao::Inquilino, "2026-0a-10");
    }, "letra na data deveria ser recusada");
    // 29/02 existe em ano bissexto
    moradores.cadastrar("Duda", "15350946056", "81922222222", apto,
                        TipoOcupacao::Inquilino, "2028-02-29");

    // bug 2: nome so com espacos vira ErroValidacao (nao ErroBanco)
    exigirErro<ErroValidacao>([&] {
        moradores.cadastrar("   ", "16899535009", "81911111111", apto,
                            TipoOcupacao::Inquilino, "2026-01-01");
    }, "nome so com espacos deveria ser ErroValidacao");

    // bug 3: erros de chave estrangeira viram regra de negocio
    exigirErro<ErroRegraNegocio>([&] { visitas.registrarEntrada(visitante, 9999, porteiro); },
                                 "entrada em apartamento inexistente");
    exigirErro<ErroRegraNegocio>([&] {
        reservas.criar(9999, area, "2099-01-10", "10:00", "12:00", 10);
    }, "reserva com morador inexistente");
    exigirErro<ErroRegraNegocio>([&] {
        reservas.criar(porteiro, area, "2099-01-10", "10:00", "12:00", 10);
    }, "reserva com id de funcionario no lugar do morador");

    // bug 5: reserva em data passada
    exigirErro<ErroRegraNegocio>([&] {
        reservas.criar(morador, area, "2020-01-10", "10:00", "12:00", 10);
    }, "reserva em data passada");

    // reserva valida e remocao de area com reserva
    reservas.criar(morador, area, "2099-01-10", "10:00", "12:00", 10);
    exigirErro<ErroRegraNegocio>([&] { areas.remover(area); },
                                 "remover area que tem reserva");

    // bug 4 (nucleo): cancelar id inexistente continua devolvendo false
    exigir(!reservas.cancelar(9999), "cancelar reserva inexistente deveria devolver false");

    if (falhas == 0) std::cout << "Todos os testes de correcoes passaram.\n";
    return falhas == 0 ? 0 : 1;
}
