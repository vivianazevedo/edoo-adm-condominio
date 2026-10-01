#pragma once

#include <vector>
#include <memory>
#include "repositorio/IRepositorio.h"
#include "modelo/Reserva.h"
#include "infra/Database.h"

class RepositorioReserva : public IRepositorio<Reserva> {
private:
    Database& db_;

public:
    RepositorioReserva() : db_(Database::getInstancia()) {}

    int inserir(const Reserva& entidade) override;
    std::unique_ptr<Reserva> buscarPorId(int id) override;
    std::vector<std::unique_ptr<Reserva>> listar() override;
    bool atualizar(const Reserva& entidade) override;
    bool remover(int id) override;

    // Métodos específicos de consulta exigidos na US06
    std::vector<std::unique_ptr<Reserva>> buscarPorMorador(int moradorId);
    std::vector<std::unique_ptr<Reserva>> buscarPorAreaEData(int areaId, const std::string& data);
};