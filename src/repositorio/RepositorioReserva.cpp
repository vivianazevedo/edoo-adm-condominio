#include "repositorio/RepositorioReserva.h"
#include <iostream>

int RepositorioReserva::inserir(const Reserva& entidade) {
    std::string sql = "INSERT INTO reserva (morador_id, area_id, data, hora_inicio, hora_fim, num_convidados, status, valor) VALUES (" +
                      std::to_string(entidade.getMoradorId()) + ", " +
                      std::to_string(entidade.getAreaId()) + ", '" +
                      entidade.getData() + "', '" +
                      entidade.getHoraInicio() + "', '" +
                      entidade.getHoraFim() + "', " +
                      std::to_string(entidade.getNumConvidados()) + ", " +
                      std::to_string(static_cast<int>(entidade.getStatus())) + ", " +
                      std::to_string(entidade.getValor()) + ");";

    return db_.executarComId(sql);
}

std::unique_ptr<Reserva> RepositorioReserva::buscarPorId(int id) {
    auto lista = listar();
    for (auto& r : lista) {
        if (r->getId() == id) {
            return std::move(r);
        }
    }
    return nullptr;
}

std::vector<std::unique_ptr<Reserva>> RepositorioReserva::listar() {
    std::vector<std::unique_ptr<Reserva>> lista;
    std::string sql = "SELECT id, morador_id, area_id, data, hora_inicio, hora_fim, num_convidados, status, valor FROM reserva;";

    auto resultados = db_.consultar(sql);
    for (const auto& linha : resultados) {
        int id = std::stoi(linha.at("id"));
        int moradorId = std::stoi(linha.at("morador_id"));
        int areaId = std::stoi(linha.at("area_id"));
        std::string data = linha.at("data");
        std::string horaInicio = linha.at("hora_inicio");
        std::string horaFim = linha.at("hora_fim");
        int numConvidados = std::stoi(linha.at("num_convidados"));
        StatusReserva status = static_cast<StatusReserva>(std::stoi(linha.at("status")));
        double valor = std::stod(linha.at("valor"));

        lista.push_back(std::make_unique<Reserva>(id, moradorId, areaId, data, horaInicio, horaFim, numConvidados, status, valor));
    }
    return lista;
}

bool RepositorioReserva::atualizar(const Reserva& entidade) {
    std::string sql = "UPDATE reserva SET status = " + std::to_string(static_cast<int>(entidade.getStatus())) +
                      ", valor = " + std::to_string(entidade.getValor()) +
                      " WHERE id = " + std::to_string(entidade.getId()) + ";";
    return db_.executar(sql);
}

bool RepositorioReserva::remover(int id) {
    std::string sql = "DELETE FROM reserva WHERE id = " + std::to_string(id) + ";";
    return db_.executar(sql);
}

std::vector<std::unique_ptr<Reserva>> RepositorioReserva::buscarPorMorador(int moradorId) {
    std::vector<std::unique_ptr<Reserva>> filtrada;
    auto todas = listar();
    for (auto& r : todas) {
        if (r->getMoradorId() == moradorId) {
            filtrada.push_back(std::move(r));
        }
    }
    return filtrada;
}

std::vector<std::unique_ptr<Reserva>> RepositorioReserva::buscarPorAreaEData(int areaId, const std::string& data) {
    std::vector<std::unique_ptr<Reserva>> filtrada;
    auto todas = listar();
    for (auto& r : todas) {
        if (r->getAreaId() == areaId && r->getData() == data) {
            filtrada.push_back(std::move(r));
        }
    }
    return filtrada;
}