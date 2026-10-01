#include "infra/Database.h"
#include "infra/ErroCondominio.h"
#include "modelo/Churrasqueira.h"
#include "modelo/Piscina.h"
#include "modelo/Reserva.h"
#include "modelo/SalaoFestas.h"
#include "repositorio/RepositorioAreaComum.h"
#include "repositorio/RepositorioReserva.h"

#include <stdexcept>

namespace {
void exigir(bool ok, const char* mensagem) {
    if (!ok) throw std::runtime_error(mensagem);
}
}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) throw std::invalid_argument("Informe sql/schema.sql");
    Database& db = Database::instancia(":memory:", argv[1]);
    RepositorioAreaComum areas;
    RepositorioReserva reservas;

    const int salaoId = areas.inserir(SalaoFestas(0, "Salao d'Ana", 80, 250.0,
                                                   "08:00", "23:00"));
    const int piscinaId = areas.inserir(Piscina(0, "Piscina", 30, 0.0,
                                                "08:00", "22:00"));
    const int churrasqueiraId = areas.inserir(Churrasqueira(0, "Churrasqueira", 20,
                                                             0.0, "10:00", "22:00"));
    exigir(dynamic_cast<SalaoFestas*>(areas.buscarPorId(salaoId).get()) != nullptr,
           "Fabrica nao reconstruiu salao");
    exigir(dynamic_cast<Piscina*>(areas.buscarPorId(piscinaId).get()) != nullptr,
           "Fabrica nao reconstruiu piscina");
    exigir(dynamic_cast<Churrasqueira*>(areas.buscarPorId(churrasqueiraId).get()) != nullptr,
           "Fabrica nao reconstruiu churrasqueira");
    exigir(areas.buscarPorId(salaoId)->getNome() == "Salao d'Ana",
           "Nome com apostrofo nao persistiu");
    exigir(areas.listar().size() == 3, "Listagem de areas incompleta");

    exigir(areas.atualizar(SalaoFestas(salaoId, "Salao renovado", 70, 260.0,
                                        "09:00", "22:00")), "Area nao atualizada");
    exigir(areas.buscarPorId(salaoId)->getCapacidade() == 70,
           "Atualizacao da area nao persistiu");

    db.executar("INSERT INTO apartamento (id, bloco, numero, andar) "
               "VALUES (1, 'A', '101', 1)");
    db.executar("INSERT INTO pessoa (id, nome, cpf, telefone, tipo) "
               "VALUES (1, 'Ana Souza', '52998224725', '81999999999', 'morador')");
    db.executar("INSERT INTO morador (pessoa_id, apartamento_id, tipo_ocupacao, data_entrada) "
               "VALUES (1, 1, 'proprietario', '2026-09-01')");

    const int reservaId = reservas.inserir(Reserva(0, 1, salaoId, "2026-10-03",
        "10:00", "12:00", 4, StatusReserva::ATIVA, 272.0));
    exigir(reservas.buscarPorId(reservaId)->getStatus() == StatusReserva::ATIVA,
           "Status ativo nao foi lido do banco");
    exigir(reservas.buscarPorMorador(1).size() == 1 &&
               reservas.buscarPorAreaEData(salaoId, "2026-10-03").size() == 1,
           "Consultas de reserva incorretas");

    Reserva cancelada(reservaId, 1, salaoId, "2026-10-03", "10:00", "12:00",
                      4, StatusReserva::CANCELADA, 272.0);
    exigir(reservas.atualizar(cancelada), "Reserva nao atualizada");
    exigir(reservas.buscarPorId(reservaId)->getStatus() == StatusReserva::CANCELADA,
           "Status cancelado nao persistiu");

    bool chaveEstrangeiraProtegida = false;
    try { areas.remover(salaoId); }
    catch (const ErroBanco&) { chaveEstrangeiraProtegida = true; }
    exigir(chaveEstrangeiraProtegida, "Area com reserva foi removida");
    exigir(reservas.remover(reservaId), "Reserva nao removida");
    exigir(areas.remover(salaoId) && areas.remover(piscinaId) &&
               areas.remover(churrasqueiraId), "Areas nao removidas");
}
