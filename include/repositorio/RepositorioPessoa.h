#pragma once
#include <memory>
#include <vector>
#include "modelo/Pessoa.h"
#include "repositorio/IRepositorio.h"

using namespace std;

// guarda e le pessoas no sqlite 
// a tabela pessoa tem os dados comuns e cada tipo tem sua tabela extra
// morador -> tabela morador, funcionario -> tabela funcionario, visitante so usa pessoa
class RepositorioPessoa : public IRepositorio<Pessoa> {
public:
    int inserir(const Pessoa& pessoa) override;
    unique_ptr<Pessoa> buscarPorId(int id) override;
    vector<unique_ptr<Pessoa>> listar() override;
    bool atualizar(const Pessoa& pessoa) override;
    bool remover(int id) override;
};