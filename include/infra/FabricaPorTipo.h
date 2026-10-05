#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

// fabrica generica: guarda uma funcao que cria cada tipo, escolhida pelo texto salvo no banco (padrao factory)
// a classe Base pode ser abstrata (ex: Pessoa), quem cria o objeto de verdade e cada subclasse
template <typename Base>
class FabricaPorTipo {
public:
    // o tipo Criador e uma funcao sem parametros que devolve um objeto novo (unique_ptr)
    using Criador = std::function<std::unique_ptr<Base>()>;

    // procura o tipo no mapa e chama a funcao criadora, se o tipo nao existir joga erro
    std::unique_ptr<Base> criar(const std::string& tipo) const {
        const auto encontrado = criadores_.find(tipo);
        if (encontrado == criadores_.end()) {
            throw std::invalid_argument("Tipo nao registrado: " + tipo);
        }
        auto entidade = encontrado->second();
        if (!entidade) {
            throw std::runtime_error("Criador retornou objeto nulo: " + tipo);
        }
        return entidade;
    }

protected:
    // guarda a funcao criadora de um tipo (protected: so as fabricas filhas chamam), nao aceita tipo repetido
    void registrar(const std::string& tipo, Criador criador) {
        if (!criador) {
            throw std::invalid_argument("Criador vazio: " + tipo);
        }
        const auto inserido = criadores_.emplace(tipo, std::move(criador));
        if (!inserido.second) {
            throw std::invalid_argument("Tipo ja registrado: " + tipo);
        }
    }

private:
    // mapa: texto do tipo -> funcao que cria ele
    std::unordered_map<std::string, Criador> criadores_;
};
