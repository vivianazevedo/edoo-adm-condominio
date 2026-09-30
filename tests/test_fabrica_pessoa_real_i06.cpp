#include "infra/FabricaPessoa.h"
#include "modelo/Morador.h"

#include <memory>
#include <stdexcept>
#include <string>

int main() {
    // Estes valores representam os campos que RepositorioPessoa le do SQLite.
    const std::string tipoBanco = "morador";
    const std::string nome = "Ana Souza";
    const std::string cpf = "52998224725";
    const std::string telefone = "81999999999";
    const int apartamentoId = 1;

    FabricaPessoa fabrica;
    fabrica.registrarMorador([&] {
        return std::make_unique<Morador>(
            nome, cpf, telefone, apartamentoId,
            TipoOcupacao::Proprietario, "2026-09-01", 42);
    });

    std::unique_ptr<Pessoa> pessoa = fabrica.criar(tipoBanco);
    if (pessoa->tipo() != tipoBanco || pessoa->id() != 42 ||
        pessoa->nome() != nome || pessoa->cpf() != cpf) {
        throw std::runtime_error("FabricaPessoa nao preservou os dados do Morador");
    }

    const auto* morador = dynamic_cast<const Morador*>(pessoa.get());
    if (!morador || morador->apartamentoId() != apartamentoId ||
        morador->tipoOcupacao() != TipoOcupacao::Proprietario) {
        throw std::runtime_error("FabricaPessoa nao criou um Morador real");
    }
}
