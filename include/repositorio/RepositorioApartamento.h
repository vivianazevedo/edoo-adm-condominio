#pragma once
#include <memory>
#include <vector>
#include "modelo/Apartamento.h"
#include "repositorio/IRepositorio.h"


// guarda e le apartamentos no sqlite
// nao carrega os moradores, quem liga eles ao apartamento e o service
class RepositorioApartamento : public IRepositorio<Apartamento> {
public:
    // os cinco metodos do CRUD da IRepositorio, escritos com sql no .cpp
    int inserir(const Apartamento& apartamento) override;
    std::unique_ptr<Apartamento> buscarPorId(int id) override;
    std::vector<std::unique_ptr<Apartamento>> listar() override;
    bool atualizar(const Apartamento& apartamento) override;
    bool remover(int id) override;
};