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

class ReservaService {
private:
    std::shared_ptr<IRepositorioReserva> repoReserva_;
    std::shared_ptr<IRepositorioAreaComum> repoArea_;
    std::function<std::chrono::system_clock::time_point()> agora_;

public:
    ReservaService();
    ReservaService(std::shared_ptr<IRepositorioReserva> repoReserva,
                   std::shared_ptr<IRepositorioAreaComum> repoArea,
                   std::function<std::chrono::system_clock::time_point()> agora =
                       std::chrono::system_clock::now);

    int criar(int moradorId, int areaId, const std::string& data, 
              const std::string& horaInicio, const std::string& horaFim, int convidados);

    bool cancelar(int reservaId);

    std::vector<Reserva> listarPorMorador(int moradorId);

    std::vector<Reserva> listarPorArea(int areaId, const std::string& data);
};

#endif
