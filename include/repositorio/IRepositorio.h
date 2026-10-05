#pragma once

#include <memory>
#include <vector>

// interface generica do CRUD, cada repositorio implementa pra sua entidade T (padrao repository)
// e uma classe abstrata: todos os metodos sao virtuais puros
template <typename T>
class IRepositorio {
public:
    // destrutor virtual
    virtual ~IRepositorio() = default;

    // create: salva no banco e devolve o id gerado
    virtual int inserir(const T& entidade) = 0;

    // read: busca pelo id, devolve nullptr se nao achar
    virtual std::unique_ptr<T> buscarPorId(int id) = 0;

    // read: devolve todas, em unique_ptr pra guardar o tipo real (Morador, Piscina...)
    virtual std::vector<std::unique_ptr<T>> listar() = 0;

    // update: atualiza pelo id, devolve false se o id nao existe
    virtual bool atualizar(const T& entidade) = 0;

    // delete: remove pelo id, devolve false se o id nao existe
    virtual bool remover(int id) = 0;
};
