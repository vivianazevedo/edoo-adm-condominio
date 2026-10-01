#pragma once

#include "modelo/AreaComum.h"

class Churrasqueira final : public AreaComum {
public:
    using AreaComum::AreaComum;
    std::string getTipo() const override;
    double calcularTaxa(int convidados) const override;
    bool validarReserva(int convidados, const std::string& inicio,
                        const std::string& fim) const override;
};
