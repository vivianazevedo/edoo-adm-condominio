#include "servico/ReservaService.h"
#include "repositorio/RepositorioReserva.h"
#include "repositorio/RepositorioAreaComum.h"

int ReservaService::criar(int moradorId, int areaId, const std::string& data, 
                          const std::string& horaInicio, const std::string& horaFim, int convidados) {
    RepositorioAreaComum repoArea;
    auto area = repoArea.buscarPorId(areaId);
    
    // 1. Valida se a área existe
    if (!area) {
        return -1;
    }

    // 2. Validação polimórfica (regras de convidados/capacidade da área específica)
    if (!area->validarReserva(convidados, horaInicio, horaFim)) {
        return -1;
    }

    RepositorioReserva repoReserva;

    // 3. Validação de sobreposição de horário (regra de negócio da US06)
    auto existentes = repoReserva.buscarPorAreaEData(areaId, data);
    for (const auto& r : existentes) {
        if (r->getStatus() == StatusReserva::ATIVA) {
            if ((horaInicio >= r->getHoraInicio() && horaInicio < r->getHoraFim()) ||
                (horaFim > r->getHoraInicio() && horaFim <= r->getHoraFim())) {
                return -1; // Conflito de horário detectado!
            }
        }
    }

    // 4. Cálculo polimórfico do valor da reserva
    double valorTotal = area->calcularTaxa(convidados);

    // 5. Inserção no banco de dados
    Reserva novaReserva(0, moradorId, areaId, data, horaInicio, horaFim, convidados, StatusReserva::ATIVA, valorTotal);
    return repoReserva.inserir(novaReserva);
}

bool ReservaService::cancelar(int reservaId) {
    RepositorioReserva repo;
    auto reserva = repo.buscarPorId(reservaId);
    if (!reserva) return false;

    reserva->setStatus(StatusReserva::CANCELADA);
    return repo.atualizar(*reserva);
}

std::vector<std::unique_ptr<Reserva>> ReservaService::listarPorMorador(int moradorId) {
    RepositorioReserva repo;
    return repo.buscarPorMorador(moradorId);
}

std::vector<std::unique_ptr<Reserva>> ReservaService::listarPorArea(int areaId, const std::string& data) {
    RepositorioReserva repo;
    return repo.buscarPorAreaEData(areaId, data);
}