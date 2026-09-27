#include "repositorio/IRepositorio.h"

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct RegistroExemplo {
    int id;
    std::string nome;
};

// Exemplo pequeno de implementacao do contrato; os repositorios reais usarao SQLite.
class RepositorioExemplo final : public IRepositorio<RegistroExemplo> {
public:
    int inserir(const RegistroExemplo& entidade) override {
        const int id = proximoId_++;
        dados_[id] = RegistroExemplo{id, entidade.nome};
        return id;
    }

    std::unique_ptr<RegistroExemplo> buscarPorId(int id) override {
        const auto encontrado = dados_.find(id);
        if (encontrado == dados_.end()) {
            return nullptr;
        }
        return std::make_unique<RegistroExemplo>(encontrado->second);
    }

    std::vector<std::unique_ptr<RegistroExemplo>> listar() override {
        std::vector<std::unique_ptr<RegistroExemplo>> resultado;
        for (const auto& [id, entidade] : dados_) {
            (void)id;
            resultado.push_back(std::make_unique<RegistroExemplo>(entidade));
        }
        return resultado;
    }

    bool atualizar(const RegistroExemplo& entidade) override {
        const auto encontrado = dados_.find(entidade.id);
        if (encontrado == dados_.end()) {
            return false;
        }
        encontrado->second = entidade;
        return true;
    }

    bool remover(int id) override {
        return dados_.erase(id) == 1;
    }

private:
    int proximoId_ = 1;
    std::map<int, RegistroExemplo> dados_;
};

int main() {
    RepositorioExemplo repositorio;
    IRepositorio<RegistroExemplo>& contrato = repositorio;

    const int id = contrato.inserir({0, "Morador de exemplo"});
    assert(id > 0);
    assert(contrato.buscarPorId(id)->nome == "Morador de exemplo");
    assert(contrato.buscarPorId(999) == nullptr);
    assert(contrato.listar().size() == 1);
    assert(contrato.atualizar({id, "Nome atualizado"}));
    assert(contrato.buscarPorId(id)->nome == "Nome atualizado");
    assert(!contrato.atualizar({999, "Inexistente"}));
    assert(contrato.remover(id));
    assert(!contrato.remover(id));
    assert(contrato.listar().empty());
}
