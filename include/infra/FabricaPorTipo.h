#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

// Seleciona um construtor registrado pelo discriminador salvo no banco.
// Base pode ser abstrata; a entidade concreta e construida pelo modulo dono.
template <typename Base>
class FabricaPorTipo {
public:
    using Criador = std::function<std::unique_ptr<Base>()>;

    // Cria a subclasse correspondente ou rejeita um tipo desconhecido.
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
    // Registra um construtor para um dos tipos permitidos pela fabrica concreta.
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
    std::unordered_map<std::string, Criador> criadores_;
};
