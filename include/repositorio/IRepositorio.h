#pragma once

#include <memory>
#include <vector>

// Contrato de persistencia usado pelos repositorios de cada entidade.
template <typename T>
class IRepositorio {
public:
    virtual ~IRepositorio() = default;

    // Insere a entidade e devolve seu identificador no banco.
    virtual int inserir(const T& entidade) = 0;

    // Busca uma entidade pelo identificador; devolve nullptr se nao existir.
    virtual std::unique_ptr<T> buscarPorId(int id) = 0;

    // Devolve todas as entidades, preservando o tipo concreto das subclasses.
    virtual std::vector<std::unique_ptr<T>> listar() = 0;

    // Atualiza uma entidade existente; devolve false se nao existir.
    virtual bool atualizar(const T& entidade) = 0;

    // Remove pelo identificador; devolve false se nao existir.
    virtual bool remover(int id) = 0;
};
