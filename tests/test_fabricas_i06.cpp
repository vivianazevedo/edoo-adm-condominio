// teste das fabricas com classes falsas minimas: cada tipo cria a subclasse certa, tipo desconhecido ou repetido da erro
#include <memory>
#include <stdexcept>
#include <string>

// modelos minimos pra testar a selecao sem alterar as classes da equipe
class Pessoa {
public:
    virtual ~Pessoa() = default;
    virtual const char* tipo() const = 0;
};

class Morador final : public Pessoa {
public:
    const char* tipo() const override { return "morador"; }
};

class Visitante final : public Pessoa {
public:
    const char* tipo() const override { return "visitante"; }
};

class Funcionario final : public Pessoa {
public:
    const char* tipo() const override { return "funcionario"; }
};

class AreaComum {
public:
    virtual ~AreaComum() = default;
    virtual const char* tipo() const = 0;
};

class SalaoFestas final : public AreaComum {
public:
    const char* tipo() const override { return "SalaoFestas"; }
};

class Piscina final : public AreaComum {
public:
    const char* tipo() const override { return "Piscina"; }
};

class Churrasqueira final : public AreaComum {
public:
    const char* tipo() const override { return "Churrasqueira"; }
};

#include "infra/FabricaAreaComum.h"
#include "infra/FabricaPessoa.h"

// joga excecao se a condicao for falsa, e assim que o teste falha
void exigir(bool condicao, const char* mensagem) {
    if (!condicao) {
        throw std::runtime_error(mensagem);
    }
}

int main() {
    FabricaPessoa pessoas;
    pessoas.registrarMorador([] { return std::make_unique<Morador>(); });
    pessoas.registrarVisitante([] { return std::make_unique<Visitante>(); });
    pessoas.registrarFuncionario([] { return std::make_unique<Funcionario>(); });
    exigir(std::string(pessoas.criar("morador")->tipo()) == "morador", "Morador incorreto");
    exigir(std::string(pessoas.criar("visitante")->tipo()) == "visitante", "Visitante incorreto");
    exigir(std::string(pessoas.criar("funcionario")->tipo()) == "funcionario", "Funcionario incorreto");

    FabricaAreaComum areas;
    areas.registrarSalaoFestas([] { return std::make_unique<SalaoFestas>(); });
    areas.registrarPiscina([] { return std::make_unique<Piscina>(); });
    areas.registrarChurrasqueira([] { return std::make_unique<Churrasqueira>(); });
    exigir(std::string(areas.criar("SalaoFestas")->tipo()) == "SalaoFestas", "Salao incorreto");
    exigir(std::string(areas.criar("Piscina")->tipo()) == "Piscina", "Piscina incorreta");
    exigir(std::string(areas.criar("Churrasqueira")->tipo()) == "Churrasqueira", "Churrasqueira incorreta");

    bool tipoInvalido = false;
    try {
        pessoas.criar("porteiro");
    } catch (const std::invalid_argument&) {
        tipoInvalido = true;
    }
    exigir(tipoInvalido, "Tipo desconhecido deveria falhar");

    bool duplicado = false;
    try {
        pessoas.registrarMorador([] { return std::make_unique<Morador>(); });
    } catch (const std::invalid_argument&) {
        duplicado = true;
    }
    exigir(duplicado, "Registro duplicado deveria falhar");
}
