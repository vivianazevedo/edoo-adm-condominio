#pragma once
#include "modelo/AreaComum.h"

class Churrasqueira : public AreaComum {
public:
    Churrasqueira(int id, const std::string& nome, int capacidade, double taxaBase,
                  const std::string& horaAbertura, const std::string& horaFechamento)
        : AreaComum(id, nome, "Churrasqueira", capacidade, taxaBase, horaAbertura, horaFechamento) {}

    double calcularTaxa(int numConvidados) const override {
        return taxaBase_; // Taxa fixa de R$ 40
    }

    bool validarReserva(int numConvidados, const std::string& horaInicio, const std::string& horaFim) const override {
        if (numConvidados < 0 || numConvidados > capacidade_) {
            return false;
        }
        return true;
    }
};