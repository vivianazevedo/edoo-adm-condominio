#pragma once
#include "modelo/AreaComum.h"

class SalaoFestas : public AreaComum {
public:
    SalaoFestas(int id, const std::string& nome, int capacidade, double taxaBase,
                const std::string& horaAbertura, const std::string& horaFechamento)
        : AreaComum(id, nome, "SalaoFestas", capacidade, taxaBase, horaAbertura, horaFechamento) {}

    double calcularTaxa(int numConvidados) const override {
        // Taxa base + R$ 3 por convidado (RN de polimorfismo)
        return taxaBase_ + (numConvidados * 3.0);
    }

    bool validarReserva(int numConvidados, const std::string& horaInicio, const std::string& horaFim) const override {
        if (numConvidados < 0 || numConvidados > capacidade_) {
            return false;
        }
        return true;
    }
};