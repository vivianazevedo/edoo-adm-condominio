#pragma once

#include "modelo/AreaComum.h"
#include "repositorio/IRepositorioAreaComum.h"

#include <memory>
#include <string>
#include <vector>

class AreaComumService {
public:
    AreaComumService();
    explicit AreaComumService(std::shared_ptr<IRepositorioAreaComum> repo);

    int cadastrar(const std::string& tipo, const std::string& nome, int capacidade,
                  double taxaBase, const std::string& abertura,
                  const std::string& fechamento);
    std::vector<std::unique_ptr<AreaComum>> listar();
    bool editar(int id, const std::string& nome, int capacidade, double taxaBase);
    bool remover(int id);

private:
    std::shared_ptr<IRepositorioAreaComum> repo_;
};
