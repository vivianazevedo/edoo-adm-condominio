#include "servico/ReservaService.h"
#include "repositorio/IRepositorioReserva.h"
#include "repositorio/IRepositorioAreaComum.h"
#include "modelo/Reserva.h"
#include "modelo/AreaComum.h"
#include "infra/ErroCondominio.h"

#include <string>
#include <vector>
#include <memory>

ReservaService::ReservaService(std::shared_ptr<RepositorioReserva> repoReserva,
                               std::shared_ptr<IRepositorioAreaComum> repoArea)
    : repoReserva_(repoReserva), repoArea_(repoArea) {}

int ReservaService::criar(int moradorId, int areaId, const std::string& data, 
                          const std::string& horaInicio, const std::string& horaFim, int convidados) {
    if (moradorId <= 0 || areaId <= 0) {
        throw ErroValidacao("IDs de morador e área comum devem ser válidos.");
    }

    auto area = repoArea_->buscarPorId(areaId);
    if (!area) {
        throw ErroRegraNegocio("Área comum não encontrada.");
    }

    if (!area->validarReserva(convidados, horaInicio, horaFim)) {
        throw ErroRegraNegocio("Capacidade ou horário inválido para a área selecionada.");
    }

    auto existentes = repoReserva_->buscarPorAreaEData(areaId, data);
    for (const auto& r : existentes) {
        if (r && r->getStatus() == StatusReserva::ATIVA) {
            if ((horaInicio >= r->getHoraInicio() && horaInicio < r->getHoraFim()) ||
                (horaFim > r->getHoraInicio() && horaFim <= r->getHoraFim())) {
                throw ErroRegraNegocio("Conflito de horário! Já existe uma reserva ativa neste período.");
            }
        }
    }

    double valorTotal = area->calcularTaxa(convidados);

    Reserva novaReserva(0, moradorId, areaId, data, horaInicio, horaFim, convidados, StatusReserva::ATIVA, valorTotal);
    return repoReserva_->inserir(novaReserva);
}

bool ReservaService::cancelar(int reservaId) {
    if (reservaId <= 0) {
        throw ErroValidacao("ID de reserva inválido.");
    }

    auto reserva = repoReserva_->buscarPorId(reservaId);
    if (!reserva) {
        throw ErroRegraNegocio("Reserva não encontrada.");
    }

    reserva->setStatus(StatusReserva::CANCELADA);
    return repoReserva_->atualizar(*reserva);
}

std::vector<Reserva> ReservaService::listarPorMorador(int moradorId) {
    if (moradorId <= 0) {
        throw ErroValidacao("ID de morador inválido.");
    }

    auto ponteiros = repoReserva_->buscarPorMorador(moradorId);
    std::vector<Reserva> resultado;
    resultado.reserve(ponteiros.size());
    for (const auto& ptr : ponteiros) {
        if (ptr) {
            resultado.push_back(*ptr);
        }
    }
    return resultado;
}

std::vector<Reserva> ReservaService::listarPorArea(int areaId, const std::string& data) {
    if (areaId <= 0) {
        throw ErroValidacao("ID de área inválido.");
    }

    auto ponteiros = repoReserva_->buscarPorAreaEData(areaId, data);
    std::vector<Reserva> resultado;
    resultado.reserve(ponteiros.size());
    for (const auto& ptr : ponteiros) {
        if (ptr) {
            resultado.push_back(*ptr);
        }
    }
    return resultado;
}