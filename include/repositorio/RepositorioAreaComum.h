#pragma once

#include <vector>
#include <memory>
#include "repositorio/IRepositorioAreaComum.h"
#include "modelo/AreaComum.h"
#include "infra/Database.h"

// guarda e le areas comuns no sqlite
// na leitura usa a FabricaAreaComum pra criar a subclasse certa (SalaoFestas, Piscina ou Churrasqueira)
class RepositorioAreaComum : public IRepositorioAreaComum {
public:
    // os cinco metodos do CRUD da IRepositorio, escritos com sql no .cpp
    int inserir(const AreaComum& entidade) override;
    std::unique_ptr<AreaComum> buscarPorId(int id) override;
    std::vector<std::unique_ptr<AreaComum>> listar() override;
    bool atualizar(const AreaComum& entidade) override;
    bool remover(int id) override;
};

