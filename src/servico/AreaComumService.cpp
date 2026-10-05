#include "servico/AreaComumService.h"

#include "infra/ErroCondominio.h"
#include "modelo/Churrasqueira.h"
#include "modelo/Piscina.h"
#include "modelo/SalaoFestas.h"
#include "repositorio/RepositorioAreaComum.h"

#include <memory>
#include <utility>

AreaComumService::AreaComumService()
    : repo_(std::make_shared<RepositorioAreaComum>()) {}

AreaComumService::AreaComumService(std::shared_ptr<IRepositorioAreaComum> repo)
    : repo_(std::move(repo)) {
    if (!repo_) throw ErroValidacao("Repositorio de areas nao informado");
}

int AreaComumService::cadastrar(const std::string& tipo, const std::string& nome,
                                int capacidade, double taxaBase,
                                const std::string& abertura,
                                const std::string& fechamento) {
    if (tipo == "SalaoFestas") {
        return repo_->inserir(SalaoFestas(0, nome, capacidade, taxaBase,
                                         abertura, fechamento));
    }
    if (tipo == "Piscina") {
        return repo_->inserir(Piscina(0, nome, capacidade, taxaBase,
                                     abertura, fechamento));
    }
    if (tipo == "Churrasqueira") {
        return repo_->inserir(Churrasqueira(0, nome, capacidade, taxaBase,
                                           abertura, fechamento));
    }
    throw ErroValidacao("Tipo de area comum desconhecido: " + tipo);
}

std::vector<std::unique_ptr<AreaComum>> AreaComumService::listar() {
    return repo_->listar();
}

bool AreaComumService::editar(int id, const std::string& nome, int capacidade,
                              double taxaBase) {
    auto atual = repo_->buscarPorId(id);
    if (!atual) return false;
    const auto& abertura = atual->getHoraAbertura();
    const auto& fechamento = atual->getHoraFechamento();
    const std::string tipo = atual->getTipo();
    if (tipo == "SalaoFestas") {
        return repo_->atualizar(SalaoFestas(id, nome, capacidade, taxaBase,
                                           abertura, fechamento));
    }
    if (tipo == "Piscina") {
        return repo_->atualizar(Piscina(id, nome, capacidade, taxaBase,
                                       abertura, fechamento));
    }
    if (tipo == "Churrasqueira") {
        return repo_->atualizar(Churrasqueira(id, nome, capacidade, taxaBase,
                                             abertura, fechamento));
    }
    throw ErroBanco("Tipo de area comum desconhecido: " + tipo);
}

bool AreaComumService::remover(int id) {
    // o banco recusa apagar area que tem reserva (chave estrangeira); avisa com regra de negocio
    try {
        return repo_->remover(id);
    } catch (const ErroBanco& e) {
        if (std::string(e.what()).find("FOREIGN KEY") != std::string::npos) {
            throw ErroRegraNegocio("Area comum com reservas nao pode ser removida");
        }
        throw;
    }
}
