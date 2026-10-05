#pragma once

#include "modelo/Reserva.h"
#include "repositorio/IRepositorio.h"

#include <string>
#include <vector>

// repositorio de reservas: alem do CRUD, tem as duas buscas que o ReservaService precisa
class IRepositorioReserva : public IRepositorio<Reserva> {
public:
    ~IRepositorioReserva() override = default;
    // todas as reservas de um morador
    virtual std::vector<std::unique_ptr<Reserva>> buscarPorMorador(int moradorId) = 0;
    // reservas de uma area em um dia (usado pra ver conflito de horario)
    virtual std::vector<std::unique_ptr<Reserva>> buscarPorAreaEData(
        int areaId, const std::string& data) = 0;
};
