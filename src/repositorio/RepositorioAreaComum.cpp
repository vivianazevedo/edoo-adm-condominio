#include "repositorio/RepositorioAreaComum.h"
#include "modelo/SalaoFestas.h"
#include "modelo/Piscina.h"
#include "modelo/Churrasqueira.h"
#include <iostream>

int RepositorioAreaComum::inserir(const AreaComum& entidade) {
    std::string sql = "INSERT INTO area_comum (nome, tipo, capacidade, taxa_base, hora_abertura, hora_fechamento) VALUES ('" +
                      entidade.getNome() + "', '" + entidade.getTipo() + "', " +
                      std::to_string(entidade.getCapacidade()) + ", " +
                      std::to_string(entidade.getTaxaBase()) + ", '" +
                      entidade.getHoraAbertura() + "', '" + entidade.getHoraFechamento() + "');";
    
    return db_.executarComId(sql);
}

std::unique_ptr<AreaComum> RepositorioAreaComum::buscarPorId(int id) {
    auto lista = listar();
    for (auto& area : lista) {
        if (area->getId() == id) {
            return std::move(area);
        }
    }
    return nullptr;
}

std::vector<std::unique_ptr<AreaComum>> RepositorioAreaComum::listar() {
    std::vector<std::unique_ptr<AreaComum>> lista;
    std::string sql = "SELECT id, nome, tipo, capacidade, taxa_base, hora_abertura, hora_fechamento FROM area_comum;";
    
    auto resultados = db_.consultar(sql);
    for (const auto& linha : resultados) {
        int id = std::stoi(linha.at("id"));
        std::string nome = linha.at("nome");
        std::string tipo = linha.at("tipo");
        int capacidade = std::stoi(linha.at("capacidade"));
        double taxaBase = std::stod(linha.at("taxa_base"));
        std::string abertura = linha.at("hora_abertura");
        std::string fechamento = linha.at("hora_fechamento");

        // Instanciação polimórfica baseada no tipo salvo no banco
        if (tipo == "SalaoFestas") {
            lista.push_back(std::make_unique<SalaoFestas>(id, nome, capacidade, taxaBase, abertura, fechamento));
        } else if (tipo == "Piscina") {
            lista.push_back(std::make_unique<Piscina>(id, nome, capacidade, taxaBase, abertura, fechamento));
        } else if (tipo == "Churrasqueira") {
            lista.push_back(std::make_unique<Churrasqueira>(id, nome, capacidade, taxaBase, abertura, fechamento));
        }
    }
    return lista;
}

bool RepositorioAreaComum::atualizar(const AreaComum& entidade) {
    std::string sql = "UPDATE area_comum SET nome = '" + entidade.getNome() +
                      "', capacidade = " + std::to_string(entidade.getCapacidade()) +
                      ", taxa_base = " + std::to_string(entidade.getTaxaBase()) +
                      " WHERE id = " + std::to_string(entidade.getId()) + ";";
    return db_.executar(sql);
}

bool RepositorioAreaComum::remover(int id) {
    std::string sql = "DELETE FROM area_comum WHERE id = " + std::to_string(id) + ";";
    return db_.executar(sql);
}