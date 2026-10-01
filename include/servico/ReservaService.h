#ifndef RESERVA_SERVICE_H
#define RESERVA_SERVICE_H

#include <memory>
#include <string>
#include <vector>

#include "repositorio/IRepositorioReserva.h"
#include "repositorio/IRepositorioAreaComum.h"
#include "modelo/Reserva.h"

class ReservaService {
private:
    std::shared_ptr<RepositorioReserva> repoReserva_;
    std::shared_ptr<IRepositorioAreaComum> repoArea_;

public:
    ReservaService(std::shared_ptr<RepositorioReserva> repoReserva,
                   std::shared_ptr<IRepositorioAreaComum> repoArea);

    int criar(int moradorId, int areaId, const std::string& data, 
              const std::string& horaInicio, const std::string& horaFim, int convidados);

    bool cancelar(int reservaId);

    std::vector<Reserva> listarPorMorador(int moradorId);

    std::vector<Reserva> listarPorArea(int areaId, const std::string& data);
};

#endif
