#include "servico/ReservaService.h"

#include "infra/ErroCondominio.h"
#include "repositorio/RepositorioAreaComum.h"
#include "repositorio/RepositorioReserva.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <utility>

namespace {

std::chrono::system_clock::time_point inicioReserva(const Reserva& reserva) {
    std::tm momento{};
    momento.tm_isdst = -1;
    std::istringstream entrada(reserva.getData() + " " + reserva.getHoraInicio());
    entrada >> std::get_time(&momento, "%Y-%m-%d %H:%M");
    if (entrada.fail()) throw ErroValidacao("Data ou horario da reserva invalido");
    const std::time_t tempo = std::mktime(&momento);
    if (tempo == static_cast<std::time_t>(-1)) {
        throw ErroValidacao("Data da reserva fora do intervalo");
    }
    return std::chrono::system_clock::from_time_t(tempo);
}

}  // namespace

ReservaService::ReservaService()
    : ReservaService(std::make_shared<RepositorioReserva>(),
                     std::make_shared<RepositorioAreaComum>()) {}

ReservaService::ReservaService(
    std::shared_ptr<IRepositorioReserva> repoReserva,
    std::shared_ptr<IRepositorioAreaComum> repoArea,
    std::function<std::chrono::system_clock::time_point()> agora)
    : repoReserva_(std::move(repoReserva)), repoArea_(std::move(repoArea)),
      agora_(std::move(agora)) {
    if (!repoReserva_ || !repoArea_ || !agora_) {
        throw ErroValidacao("Dependencias do servico de reservas incompletas");
    }
}

int ReservaService::criar(int moradorId, int areaId, const std::string& data,
                          const std::string& horaInicio,
                          const std::string& horaFim, int convidados) {
    // Valida formato da data, periodo e identificadores antes de consultar o banco.
    Reserva candidata(0, moradorId, areaId, data, horaInicio, horaFim,
                      convidados, StatusReserva::ATIVA, 0.0);
    auto area = repoArea_->buscarPorId(areaId);
    if (!area) throw ErroRegraNegocio("Area comum nao encontrada");
    if (!area->validarReserva(convidados, horaInicio, horaFim)) {
        throw ErroRegraNegocio("Capacidade, duracao ou horario fora das regras da area");
    }

    for (const auto& existente : repoReserva_->buscarPorAreaEData(areaId, data)) {
        if (existente && existente->getStatus() == StatusReserva::ATIVA &&
            horaInicio < existente->getHoraFim() &&
            horaFim > existente->getHoraInicio()) {
            throw ErroRegraNegocio("Ja existe reserva ativa nesse horario");
        }
    }

    Reserva nova(0, moradorId, areaId, data, horaInicio, horaFim,
                 convidados, StatusReserva::ATIVA, area->calcularTaxa(convidados));
    return repoReserva_->inserir(nova);
}

bool ReservaService::cancelar(int reservaId) {
    if (reservaId <= 0) throw ErroValidacao("ID de reserva invalido");
    auto reserva = repoReserva_->buscarPorId(reservaId);
    if (!reserva) return false;
    if (reserva->getStatus() == StatusReserva::CANCELADA) {
        throw ErroRegraNegocio("Reserva ja esta cancelada");
    }
    if (inicioReserva(*reserva) - agora_() < std::chrono::hours(24)) {
        throw ErroRegraNegocio("Cancelamento exige 24 horas de antecedencia");
    }
    reserva->setStatus(StatusReserva::CANCELADA);
    return repoReserva_->atualizar(*reserva);
}

std::vector<Reserva> ReservaService::listarPorMorador(int moradorId) {
    if (moradorId <= 0) throw ErroValidacao("ID de morador invalido");
    std::vector<Reserva> resultado;
    for (const auto& reserva : repoReserva_->buscarPorMorador(moradorId)) {
        if (reserva) resultado.push_back(*reserva);
    }
    return resultado;
}

std::vector<Reserva> ReservaService::listarPorArea(
    int areaId, const std::string& data) {
    if (areaId <= 0) throw ErroValidacao("ID de area invalido");
    std::vector<Reserva> resultado;
    for (const auto& reserva : repoReserva_->buscarPorAreaEData(areaId, data)) {
        if (reserva) resultado.push_back(*reserva);
    }
    return resultado;
}
