#pragma once

#include "repositorio/IRepositorioReserva.h"

class RepositorioReserva final : public IRepositorioReserva {
public:
    int inserir(const Reserva& entidade) override;
    std::unique_ptr<Reserva> buscarPorId(int id) override;
    std::vector<std::unique_ptr<Reserva>> listar() override;
    bool atualizar(const Reserva& entidade) override;
    bool remover(int id) override;
    std::vector<std::unique_ptr<Reserva>> buscarPorMorador(int moradorId) override;
    std::vector<std::unique_ptr<Reserva>> buscarPorAreaEData(
        int areaId, const std::string& data) override;
};
