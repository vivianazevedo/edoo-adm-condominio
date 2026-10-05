#pragma once

#include "modelo/AreaComum.h"

// area do tipo churrasqueira (final = ninguem herda dela)
class Churrasqueira final : public AreaComum {
public:
    // aproveita o construtor da classe base
    using AreaComum::AreaComum;
    // os tres metodos abaixo reescrevem os virtuais puros da AreaComum (override)
    std::string getTipo() const override;
    double calcularTaxa(int convidados) const override;
    bool validarReserva(int convidados, const std::string& inicio,
                        const std::string& fim) const override;
};
