#ifndef RESERVA_SERVICE_H
#define RESERVA_SERVICE_H

#include <memory>
#include <chrono>
#include <functional>
#include <string>
#include <vector>

#include "repositorio/IRepositorioReserva.h"
#include "repositorio/IRepositorioAreaComum.h"
#include "modelo/Reserva.h"

// regras das reservas: criar (conflito, capacidade, horario e valor), cancelar (24 horas) e consultar
// quem decide se pode ou nao reservar e essa classe
class ReservaService {
private:
    // repositorios guardados em shared_ptr
    std::shared_ptr<IRepositorioReserva> repoReserva_;
    std::shared_ptr<IRepositorioAreaComum> repoArea_;
    // funcao que devolve a hora de agora (nos testes da pra trocar por um horario fixo)
    std::function<std::chrono::system_clock::time_point()> agora_;

public:
    // o construtor padrao usa os repositorios reais, o outro recebe os repositorios e o relogio (testes)
    ReservaService();
    ReservaService(std::shared_ptr<IRepositorioReserva> repoReserva,
                   std::shared_ptr<IRepositorioAreaComum> repoArea,
                   std::function<std::chrono::system_clock::time_point()> agora =
                       std::chrono::system_clock::now);

    // cria a reserva e devolve o id, joga ErroRegraNegocio se nao puder (RN01 a RN03 ou data que ja passou)
    int criar(int moradorId, int areaId, const std::string& data, 
              const std::string& horaInicio, const std::string& horaFim, int convidados);

    // cancela se faltarem 24 horas ou mais (RN04), devolve false se o id nao existe
    bool cancelar(int reservaId);

    // reservas de um morador
    std::vector<Reserva> listarPorMorador(int moradorId);

    // reservas de uma area em um dia
    std::vector<Reserva> listarPorArea(int areaId, const std::string& data);
};

#endif
