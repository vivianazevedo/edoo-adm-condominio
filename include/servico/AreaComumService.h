#pragma once

#include "modelo/AreaComum.h"
#include "repositorio/IRepositorioAreaComum.h"

#include <memory>
#include <string>
#include <vector>

// regras de negocio das areas comuns: cadastrar, listar, editar e remover
class AreaComumService {
public:
    // o construtor padrao usa o repositorio real, o outro recebe um (bom pra testes)
    AreaComumService();
    explicit AreaComumService(std::shared_ptr<IRepositorioAreaComum> repo);

    // cria a subclasse certa pelo texto do tipo (SalaoFestas, Piscina ou Churrasqueira)
    int cadastrar(const std::string& tipo, const std::string& nome, int capacidade,
                  double taxaBase, const std::string& abertura,
                  const std::string& fechamento);
    std::vector<std::unique_ptr<AreaComum>> listar();
    // so muda nome, capacidade e taxa, o tipo e os horarios continuam os mesmos
    bool editar(int id, const std::string& nome, int capacidade, double taxaBase);
    // devolve false se o id nao existe, joga ErroRegraNegocio se a area tem reservas
    bool remover(int id);

private:
    // repositorio guardado em shared_ptr
    std::shared_ptr<IRepositorioAreaComum> repo_;
};
