#pragma once

#include "repositorio/IRepositorioReserva.h"

// guarda e le reservas no sqlite (final = ninguem herda dela)
class RepositorioReserva final : public IRepositorioReserva {
public:
    // os cinco metodos do CRUD da IRepositorio, escritos com sql no .cpp
    int inserir(const Reserva& entidade) override;
    std::unique_ptr<Reserva> buscarPorId(int id) override;
    std::vector<std::unique_ptr<Reserva>> listar() override;
    bool atualizar(const Reserva& entidade) override;
    bool remover(int id) override;
    // as duas buscas extras da IRepositorioReserva
    std::vector<std::unique_ptr<Reserva>> buscarPorMorador(int moradorId) override;
    std::vector<std::unique_ptr<Reserva>> buscarPorAreaEData(
        int areaId, const std::string& data) override;
};
