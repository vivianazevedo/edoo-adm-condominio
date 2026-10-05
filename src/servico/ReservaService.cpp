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

// junta a data e a hora de inicio da reserva num horario, pra comparar com a hora de agora
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

// versao padrao: usa os repositorios do sqlite
ReservaService::ReservaService()
    : ReservaService(std::make_shared<RepositorioReserva>(),
                     std::make_shared<RepositorioAreaComum>()) {}

// versao que recebe os repositorios e o relogio, recusa se faltar algum
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

// aplica as regras RN01 a RN03, recusa reserva no passado e salva, devolve o id da reserva
int ReservaService::criar(int moradorId, int areaId, const std::string& data,
                          const std::string& horaInicio,
                          const std::string& horaFim, int convidados) {
    // criar o objeto Reserva ja valida data, periodo e ids antes de mexer no banco
    Reserva candidata(0, moradorId, areaId, data, horaInicio, horaFim,
                      convidados, StatusReserva::ATIVA, 0.0);
    // nao deixa reservar em data ou horario que ja passou
    if (inicioReserva(candidata) < agora_()) {
        throw ErroRegraNegocio("Nao e possivel reservar em data ou horario que ja passou");
    }
    auto area = repoArea_->buscarPorId(areaId);
    if (!area) throw ErroRegraNegocio("Area comum nao encontrada");
    // polimorfismo: cada area (salao, piscina, churrasqueira) valida do seu jeito
    if (!area->validarReserva(convidados, horaInicio, horaFim)) {
        throw ErroRegraNegocio("Capacidade, duracao ou horario fora das regras da area");
    }

    // rn01: recusa se alguma reserva ativa da mesma area e do mesmo dia se sobrepoe
    // (comeca antes do fim da outra e termina depois do comeco da outra)
    for (const auto& existente : repoReserva_->buscarPorAreaEData(areaId, data)) {
        if (existente && existente->getStatus() == StatusReserva::ATIVA &&
            horaInicio < existente->getHoraFim() &&
            horaFim > existente->getHoraInicio()) {
            throw ErroRegraNegocio("Ja existe reserva ativa nesse horario");
        }
    }

    // o valor vem do calcularTaxa da propria area (polimorfismo de novo)
    Reserva nova(0, moradorId, areaId, data, horaInicio, horaFim,
                 convidados, StatusReserva::ATIVA, area->calcularTaxa(convidados));
    // a area ja foi conferida acima, entao se o banco recusar por chave estrangeira
    // o que sobra e o morador (traduz para mensagem de regra de negocio)
    try {
        return repoReserva_->inserir(nova);
    } catch (const ErroBanco& e) {
        if (std::string(e.what()).find("FOREIGN KEY") != std::string::npos) {
            throw ErroRegraNegocio("Morador " + std::to_string(moradorId) + " nao encontrado");
        }
        throw;
    }
}

// so cancela reserva que existe, que esta ativa e que comeca daqui a 24 horas ou mais (RN04)
bool ReservaService::cancelar(int reservaId) {
    if (reservaId <= 0) throw ErroValidacao("ID de reserva invalido");
    auto reserva = repoReserva_->buscarPorId(reservaId);
    if (!reserva) return false;
    if (reserva->getStatus() == StatusReserva::CANCELADA) {
        throw ErroRegraNegocio("Reserva ja esta cancelada");
    }
    // faltam menos de 24 horas pra comecar
    if (inicioReserva(*reserva) - agora_() < std::chrono::hours(24)) {
        throw ErroRegraNegocio("Cancelamento exige 24 horas de antecedencia");
    }
    reserva->setStatus(StatusReserva::CANCELADA);
    return repoReserva_->atualizar(*reserva);
}

// devolve copias das reservas do morador (o repositorio devolve ponteiros)
std::vector<Reserva> ReservaService::listarPorMorador(int moradorId) {
    if (moradorId <= 0) throw ErroValidacao("ID de morador invalido");
    std::vector<Reserva> resultado;
    for (const auto& reserva : repoReserva_->buscarPorMorador(moradorId)) {
        if (reserva) resultado.push_back(*reserva);
    }
    return resultado;
}

// devolve copias das reservas da area naquele dia
std::vector<Reserva> ReservaService::listarPorArea(
    int areaId, const std::string& data) {
    if (areaId <= 0) throw ErroValidacao("ID de area invalido");
    std::vector<Reserva> resultado;
    for (const auto& reserva : repoReserva_->buscarPorAreaEData(areaId, data)) {
        if (reserva) resultado.push_back(*reserva);
    }
    return resultado;
}
