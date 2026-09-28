#pragma once
#include "modelo/AreaComum.h"

class Piscina : public AreaComum {
public:
    Piscina(int id, const std::string& nome, int capacidade, double taxaBase,
            const std::string& horaAbertura, const std::string& horaFechamento)
        : AreaComum(id, nome, "Piscina", capacidade, taxaBase, horaAbertura, horaFechamento) {}

    double calcularTaxa(int numConvidados) const override {
        return 0.0; // Gratuita
    }

    bool validarReserva(int numConvidados, const std::string& horaInicio, const std::string& horaFim) const override {
        if (numConvidados < 0 || numConvidados > 4 || numConvidados > capacidade_) {
            return false; // No máximo 4 convidados por reserva
        }
        return true;
    }
};