#pragma once

#include "modelo/Reserva.h"
#include "repositorio/IRepositorio.h"

#include <string>
#include <vector>

class IRepositorioReserva : public IRepositorio<Reserva> {
public:
    ~IRepositorioReserva() override = default;
    virtual std::vector<std::unique_ptr<Reserva>> buscarPorMorador(int moradorId) = 0;
    virtual std::vector<std::unique_ptr<Reserva>> buscarPorAreaEData(
        int areaId, const std::string& data) = 0;
};
