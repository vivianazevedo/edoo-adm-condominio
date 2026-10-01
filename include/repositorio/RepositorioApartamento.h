#pragma once
#include <memory>
#include <vector>
#include "modelo/Apartamento.h"
#include "repositorio/IRepositorio.h"

using namespace std;

// guarda e le apartamentos no sqlite
// nao carrega os moradores, quem liga eles ao apartamento e o service
class RepositorioApartamento : public IRepositorio<Apartamento> {
public:
    int inserir(const Apartamento& apartamento) override;
    unique_ptr<Apartamento> buscarPorId(int id) override;
    vector<unique_ptr<Apartamento>> listar() override;
    bool atualizar(const Apartamento& apartamento) override;
    bool remover(int id) override;
};