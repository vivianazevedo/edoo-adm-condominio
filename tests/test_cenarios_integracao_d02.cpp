// os 10 cenarios de integracao D02 (descritos em docs/testes.md), usando services e repositorios reais num banco em memoria
#include "infra/Database.h"
#include "infra/ErroCondominio.h"
#include "modelo/Morador.h"
#include "modelo/Piscina.h"
#include "repositorio/RepositorioApartamento.h"
#include "repositorio/RepositorioAreaComum.h"
#include "repositorio/RepositorioPessoa.h"
#include "repositorio/RepositorioReserva.h"
#include "repositorio/RepositorioVisita.h"
#include "servico/ApartamentoService.h"
#include "servico/AreaComumService.h"
#include "servico/FuncionarioService.h"
#include "servico/MoradorService.h"
#include "servico/ReservaService.h"
#include "servico/VisitaService.h"

#include <sqlite3.h>

#include <chrono>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace {

// joga excecao se a condicao for falsa, e assim que o teste falha
void exigir(bool condicao, const char* mensagem) {
    if (!condicao) throw std::runtime_error(mensagem);
}

// espera que a acao jogue ErroRegraNegocio, se nao jogar o teste falha
void exigirRegra(const std::function<void()>& acao, const char* mensagem) {
    bool rejeitada = false;
    try { acao(); }
    catch (const ErroRegraNegocio&) { rejeitada = true; }
    exigir(rejeitada, mensagem);
}

// devolve um horario fixo em maio de 2030, assim o resultado nao depende do dia em que o teste roda
std::chrono::system_clock::time_point horarioFixo(int dia, int hora) {
    std::tm data{};
    data.tm_year = 2030 - 1900;
    data.tm_mon = 4;
    data.tm_mday = dia;
    data.tm_hour = hora;
    data.tm_isdst = -1;
    return std::chrono::system_clock::from_time_t(std::mktime(&data));
}

// roda um cenario e imprime se passou ou falhou
void executar(const char* codigo, const std::function<void()>& acao) {
    try {
        acao();
        std::cout << codigo << ": PASSOU\n";
    } catch (const std::exception& erro) {
        std::cerr << codigo << ": FALHOU: " << erro.what() << '\n';
        throw;
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) throw std::invalid_argument("Informe sql/schema.sql");
    Database& banco = Database::instancia(":memory:", argv[1]);
    RepositorioApartamento repoApartamentos;
    RepositorioPessoa repoPessoas;
    RepositorioVisita repoVisitas;
    auto repoAreas = std::make_shared<RepositorioAreaComum>();
    auto repoReservas = std::make_shared<RepositorioReserva>();
    ApartamentoService apartamentos(repoApartamentos);
    MoradorService moradores(repoPessoas, repoApartamentos);
    FuncionarioService funcionarios(repoPessoas);
    VisitaService visitas(repoPessoas, repoVisitas);
    AreaComumService areas(repoAreas);
    ReservaService reservas(repoReservas, repoAreas,
                            [] { return horarioFixo(1, 10); });

    int apartamentoId = 0;
    int moradorId = 0;
    int porteiroId = 0;
    int visitanteId = 0;
    int visitaId = 0;
    int salaoId = 0;
    int reservaId = 0;

    executar("D02-01", [&] {
        sqlite3_stmt* consulta = nullptr;
        const char* sql = "SELECT COUNT(*) FROM sqlite_master WHERE type='table' "
                          "AND name IN ('apartamento','pessoa','morador','funcionario',"
                          "'area_comum','reserva','visita')";
        exigir(sqlite3_prepare_v2(banco.conexao(), sql, -1, &consulta, nullptr) == SQLITE_OK,
               "Schema nao pode ser consultado");
        const bool tabelas = sqlite3_step(consulta) == SQLITE_ROW &&
                             sqlite3_column_int(consulta, 0) == 7;
        sqlite3_finalize(consulta);
        exigir(tabelas, "As sete tabelas do MVP nao foram criadas");
    });

    executar("D02-02", [&] {
        apartamentoId = apartamentos.cadastrar("A", "101", 1);
        apartamentos.editar(apartamentoId, "A", "102", 1);
        exigir(apartamentos.buscar(apartamentoId)->getNumero() == "102",
               "Apartamento editado nao persistiu");
    });

    executar("D02-03", [&] {
        moradorId = moradores.cadastrar("Ana Souza", "52998224725", "81999999999",
                                        apartamentoId, TipoOcupacao::Proprietario,
                                        "2026-09-01");
        auto lido = repoPessoas.buscarPorId(moradorId);
        exigir(dynamic_cast<Morador*>(lido.get()) != nullptr &&
                   moradores.listarPorApartamento(apartamentoId).size() == 1,
               "Morador nao foi reconstruido/vinculado ao apartamento");
    });

    executar("D02-04", [&] {
        porteiroId = funcionarios.cadastrar("Carla Dias", "12345678909",
                                            "81977777777", Cargo::Porteiro,
                                            "manha", "2026-09-01");
        exigirRegra([&] {
            funcionarios.cadastrar("Outra Pessoa", "52998224725", "81966666666",
                                   Cargo::Porteiro, "tarde", "2026-09-02");
        }, "CPF duplicado foi aceito entre tipos de pessoa");
    });

    executar("D02-05", [&] {
        exigirRegra([&] { apartamentos.remover(apartamentoId); },
                    "Apartamento ocupado foi removido");
        exigir(apartamentos.buscar(apartamentoId) != nullptr,
               "Apartamento desapareceu apos tentativa de remocao");
    });

    executar("D02-06", [&] {
        const int zeladorId = funcionarios.cadastrar("Diego Lima", "93541134780",
            "81955555555", Cargo::Zelador, "tarde", "2026-09-01");
        visitanteId = visitas.cadastrarVisitante("Bruno Lima", "11144477735",
                                                 "81988888888");
        exigirRegra([&] {
            visitas.registrarEntrada(visitanteId, apartamentoId, zeladorId);
        }, "Funcionario sem cargo de porteiro registrou visita");
        visitaId = visitas.registrarEntrada(visitanteId, apartamentoId, porteiroId);
        exigir(visitaId > 0, "Porteiro nao conseguiu registrar entrada");
    });

    executar("D02-07", [&] {
        exigir(visitas.listarAbertas().size() == 1, "Entrada nao ficou em aberto");
        visitas.registrarSaida(visitaId);
        exigir(visitas.listarAbertas().empty() &&
                   visitas.historicoPorApartamento(apartamentoId).size() == 1,
               "Saida ou historico de visitas incorreto");
    });

    executar("D02-08", [&] {
        salaoId = areas.cadastrar("SalaoFestas", "Salao", 80, 250.0,
                                  "08:00", "23:00");
        const int piscinaId = areas.cadastrar("Piscina", "Piscina", 30, 0.0,
                                              "08:00", "22:00");
        exigir(areas.editar(salaoId, "Salao renovado", 80, 250.0),
               "Edicao de area falhou");
        auto piscina = repoAreas->buscarPorId(piscinaId);
        exigir(dynamic_cast<Piscina*>(piscina.get()) != nullptr &&
                   areas.listar().size() == 2,
               "Factory ou listagem de areas falhou");
    });

    executar("D02-09", [&] {
        reservaId = reservas.criar(moradorId, salaoId, "2030-05-03",
                                   "10:00", "11:00", 4);
        exigirRegra([&] {
            reservas.criar(moradorId, salaoId, "2030-05-03", "09:00", "12:00", 4);
        }, "Reserva que engloba outra foi aceita");
        exigirRegra([&] {
            reservas.criar(moradorId, salaoId, "2030-05-04", "07:00", "08:00", 4);
        }, "Horario fora do funcionamento foi aceito");
        exigirRegra([&] {
            reservas.criar(moradorId, salaoId, "2030-05-04", "10:00", "11:00", 81);
        }, "Capacidade excedida foi aceita");
        exigir(repoReservas->buscarPorId(reservaId)->getValor() == 262.0,
               "Taxa da reserva incorreta");
    });

    executar("D02-10", [&] {
        exigir(reservas.cancelar(reservaId), "Cancelamento com antecedencia falhou");
        exigir(reservas.criar(moradorId, salaoId, "2030-05-03",
                             "10:00", "11:00", 4) > 0,
               "Horario da reserva cancelada nao foi liberado");
        const int proxima = reservas.criar(moradorId, salaoId, "2030-05-02",
                                           "09:00", "10:00", 1);
        exigirRegra([&] { reservas.cancelar(proxima); },
                    "Cancelamento com menos de 24 horas foi aceito");
    });
}
