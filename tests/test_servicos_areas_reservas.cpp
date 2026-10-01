#include "infra/Database.h"
#include "infra/ErroCondominio.h"
#include "repositorio/RepositorioAreaComum.h"
#include "repositorio/RepositorioReserva.h"
#include "servico/AreaComumService.h"
#include "servico/ReservaService.h"

#include <chrono>
#include <ctime>
#include <memory>
#include <stdexcept>

namespace {
void exigir(bool ok, const char* mensagem) {
    if (!ok) throw std::runtime_error(mensagem);
}

template <typename Acao>
void exigirRegra(Acao acao, const char* mensagem) {
    bool rejeitada = false;
    try { acao(); }
    catch (const ErroRegraNegocio&) { rejeitada = true; }
    exigir(rejeitada, mensagem);
}

std::chrono::system_clock::time_point horarioFixo(int dia, int hora) {
    std::tm data{};
    data.tm_year = 2030 - 1900;
    data.tm_mon = 4;
    data.tm_mday = dia;
    data.tm_hour = hora;
    data.tm_isdst = -1;
    return std::chrono::system_clock::from_time_t(std::mktime(&data));
}
}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) throw std::invalid_argument("Informe sql/schema.sql");
    Database& db = Database::instancia(":memory:", argv[1]);
    db.executar("INSERT INTO apartamento (id, bloco, numero, andar) "
               "VALUES (1, 'A', '101', 1)");
    db.executar("INSERT INTO pessoa (id, nome, cpf, telefone, tipo) "
               "VALUES (1, 'Ana Souza', '52998224725', '81999999999', 'morador')");
    db.executar("INSERT INTO morador (pessoa_id, apartamento_id, tipo_ocupacao, data_entrada) "
               "VALUES (1, 1, 'proprietario', '2026-09-01')");

    auto repoAreas = std::make_shared<RepositorioAreaComum>();
    auto repoReservas = std::make_shared<RepositorioReserva>();
    AreaComumService areas(repoAreas);
    ReservaService reservas(repoReservas, repoAreas,
                           [] { return horarioFixo(1, 10); });

    const int salaoId = areas.cadastrar("SalaoFestas", "Salao", 80, 250.0,
                                       "08:00", "23:00");
    const int piscinaId = areas.cadastrar("Piscina", "Piscina", 30, 0.0,
                                         "08:00", "22:00");
    exigir(areas.listar().size() == 2, "Cadastro/listagem de areas falhou");
    exigir(areas.editar(salaoId, "Salao renovado", 80, 250.0),
           "Edicao de area falhou");

    const int primeira = reservas.criar(1, salaoId, "2030-05-03", "10:00", "11:00", 4);
    exigir(reservas.listarPorMorador(1).size() == 1 &&
               reservas.listarPorArea(salaoId, "2030-05-03").size() == 1 &&
               repoReservas->buscarPorId(primeira)->getValor() == 262.0,
           "Criacao, consulta ou taxa da reserva incorreta");

    exigirRegra([&] { reservas.criar(1, salaoId, "2030-05-03", "09:00", "12:00", 4); },
               "RN01 aceitou reserva que engloba outra");
    exigirRegra([&] { reservas.criar(1, salaoId, "2030-05-03", "10:30", "11:30", 4); },
               "RN01 aceitou sobreposicao parcial");
    const int adjacente = reservas.criar(1, salaoId, "2030-05-03", "11:00", "12:00", 4);
    exigir(adjacente > 0, "RN01 rejeitou horarios apenas adjacentes");
    exigirRegra([&] { reservas.criar(1, salaoId, "2030-05-04", "07:00", "08:00", 4); },
               "RN02 aceitou horario fora do funcionamento");
    exigirRegra([&] { reservas.criar(1, salaoId, "2030-05-04", "10:00", "11:00", 81); },
               "RN03 aceitou convidados acima da capacidade");
    exigirRegra([&] { reservas.criar(1, piscinaId, "2030-05-04", "10:00", "11:00", 5); },
               "Limite especial da piscina nao foi aplicado");

    exigir(reservas.cancelar(primeira), "Cancelamento valido falhou");
    exigir(repoReservas->buscarPorId(primeira)->getStatus() == StatusReserva::CANCELADA,
           "Status cancelado nao persistiu");
    exigir(reservas.criar(1, salaoId, "2030-05-03", "10:00", "11:00", 4) > 0,
           "Horario de reserva cancelada nao foi liberado");

    const int proxima = reservas.criar(1, salaoId, "2030-05-02", "09:00", "10:00", 1);
    exigirRegra([&] { reservas.cancelar(proxima); },
               "RN04 permitiu cancelar com menos de 24 horas");
    const int limite = reservas.criar(1, salaoId, "2030-05-02", "10:00", "11:00", 1);
    exigir(reservas.cancelar(limite), "RN04 rejeitou cancelamento com exatas 24 horas");
}
