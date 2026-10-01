#pragma once

#include <vector>
#include <memory>
#include "repositorio/IRepositorioAreaComum.h"
#include "modelo/AreaComum.h"
#include "infra/Database.h"

class RepositorioAreaComum : public IRepositorioAreaComum {
public:
    int inserir(const AreaComum& entidade) override;
    std::unique_ptr<AreaComum> buscarPorId(int id) override;
    std::vector<std::unique_ptr<AreaComum>> listar() override;
    bool atualizar(const AreaComum& entidade) override;
    bool remover(int id) override;
};

