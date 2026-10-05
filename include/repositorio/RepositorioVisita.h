#pragma once
#include <memory>
#include <vector>
#include "modelo/Visita.h"
#include "repositorio/IRepositorio.h"


// guarda e le visitas no sqlite
// nao confere se o visitante e mesmo um visitante nem se o funcionario e porteiro,
// isso e regra de negocio e fica no service (o banco so garante que os ids existem)
class RepositorioVisita : public IRepositorio<Visita> {
public:
    int inserir(const Visita& visita) override;
    std::unique_ptr<Visita> buscarPorId(int id) override;
    std::vector<std::unique_ptr<Visita>> listar() override;
    bool atualizar(const Visita& visita) override;
    bool remover(int id) override;
};