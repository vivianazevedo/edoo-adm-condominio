#include "servico/AreaComumService.h"
#include "repositorio/RepositorioAreaComum.h"
#include "modelo/SalaoFestas.h"
#include "modelo/Piscina.h"
#include "modelo/Churrasqueira.h"

int AreaComumService::cadastrar(const std::string& tipo, const std::string& nome, int capacidade, 
                                double taxaBase, const std::string& abertura, const std::string& fechamento) {
    RepositorioAreaComum repo;

    if (tipo == "SalaoFestas") {
        SalaoFestas area(0, nome, capacidade, taxaBase, abertura, fechamento);
        return repo.inserir(area);
    } else if (tipo == "Piscina") {
        Piscina area(0, nome, capacidade, taxaBase, abertura, fechamento);
        return repo.inserir(area);
    } else if (tipo == "Churrasqueira") {
        Churrasqueira area(0, nome, capacidade, taxaBase, abertura, fechamento);
        return repo.inserir(area);
    }

    return -1;
}

std::vector<std::unique_ptr<AreaComum>> AreaComumService::listar() {
    RepositorioAreaComum repo;
    return repo.listar();
}

bool AreaComumService::editar(int id, const std::string& nome, int capacidade, double taxaBase) {
    RepositorioAreaComum repo;
    auto area = repo.buscarPorId(id);
    if (!area) return false;

    if (area->getTipo() == "SalaoFestas") {
        SalaoFestas editada(id, nome, capacidade, taxaBase, area->getHoraAbertura(), area->getHoraFechamento());
        return repo.atualizar(editada);
    } else if (area->getTipo() == "Piscina") {
        Piscina editada(id, nome, capacidade, taxaBase, area->getHoraAbertura(), area->getHoraFechamento());
        return repo.atualizar(editada);
    } else if (area->getTipo() == "Churrasqueira") {
        Churrasqueira editada(id, nome, capacidade, taxaBase, area->getHoraAbertura(), area->getHoraFechamento());
        return repo.atualizar(editada);
    }

    return false;
}

bool AreaComumService::remover(int id) {
    RepositorioAreaComum repo;
    return repo.remover(id);
}