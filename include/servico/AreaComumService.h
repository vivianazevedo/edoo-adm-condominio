#pragma once

#include <string>
#include <vector>
#include <memory>
#include "modelo/AreaComum.h"

class AreaComumService {
public:
    AreaComumService() = default;
    virtual ~AreaComumService() = default;

    virtual int cadastrar(const std::string& tipo, const std::string& nome, int capacidade, 
                          double taxaBase, const std::string& abertura, const std::string& fechamento);

    virtual std::vector<std::unique_ptr<AreaComum>> listar();

    virtual bool editar(int id, const std::string& nome, int capacidade, double taxaBase);

    virtual bool remover(int id);
};