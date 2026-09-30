#pragma once

#include <vector>
#include <memory>
#include "repositorio/IRepositorio.h"
#include "modelo/AreaComum.h"
#include "infra/Database.h"

class RepositorioAreaComum : public IRepositorio<AreaComum> {
private:
    Database& db_;

public:
    RepositorioAreaComum() : db_(Database::getInstancia()) {}

    int inserir(const AreaComum& entidade) override;
    std::unique_ptr<AreaComum> buscarPorId(int id) override;
    std::vector<std::unique_ptr<AreaComum>> listar() override;
    bool atualizar(const AreaComum& entidade) override;
    bool remover(int id) override;
};

