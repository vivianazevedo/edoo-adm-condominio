#pragma once

#include <string>
#include <vector>
#include <memory>
#include "modelo/Reserva.h"

class ReservaService {
public:
    ReservaService() = default;
    virtual ~ReservaService() = default;

    virtual int criar(int moradorId, int areaId, const std::string& data, 
                      const std::string& horaInicio, const std::string& horaFim, int convidados);

    virtual bool cancelar(int reservaId);

    virtual std::vector<std::unique_ptr<Reserva>> listarPorMorador(int moradorId);

    virtual std::vector<std::unique_ptr<Reserva>> listarPorArea(int areaId, const std::string& data);
};