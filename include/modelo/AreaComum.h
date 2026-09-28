#pragma once

#include <string>

/**
 * @brief Classe abstrata que representa uma Área Comum do condomínio.
 */
class AreaComum {
protected:
    int id_;
    std::string nome_;
    std::string tipo_;
    int capacidade_;
    double taxaBase_;
    std::string horaAbertura_;
    std::string horaFechamento_;

public:
    AreaComum(int id, const std::string& nome, const std::string& tipo, int capacidade,
              double taxaBase, const std::string& horaAbertura, const std::string& horaFechamento)
        : id_(id), nome_(nome), tipo_(tipo), capacidade_(capacidade),
          taxaBase_(taxaBase), horaAbertura_(horaAbertura), horaFechamento_(horaFechamento) {}

    virtual ~AreaComum() = default;

    // Métodos virtuais puros (Polimorfismo)
    virtual double calcularTaxa(int numConvidados) const = 0;
    virtual bool validarReserva(int numConvidados, const std::string& horaInicio, const std::string& horaFim) const = 0;

    // Getters
    int getId() const { return id_; }
    std::string getNome() const { return nome_; }
    std::string getTipo() const { return tipo_; }
    int getCapacidade() const { return capacidade_; }
    double getTaxaBase() const { return taxaBase_; }
    std::string getHoraAbertura() const { return horaAbertura_; }
    std::string getHoraFechamento() const { return horaFechamento_; }
};