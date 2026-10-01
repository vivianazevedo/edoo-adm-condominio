#pragma once

#include <string>

class AreaComum {
public:
    AreaComum(int id, std::string nome, int capacidade, double taxaBase,
              std::string horaAbertura, std::string horaFechamento);
    virtual ~AreaComum() = default;

    int getId() const noexcept { return id_; }
    const std::string& getNome() const noexcept { return nome_; }
    int getCapacidade() const noexcept { return capacidade_; }
    double getTaxaBase() const noexcept { return taxaBase_; }
    const std::string& getHoraAbertura() const noexcept { return horaAbertura_; }
    const std::string& getHoraFechamento() const noexcept { return horaFechamento_; }

    virtual std::string getTipo() const = 0;
    virtual double calcularTaxa(int convidados) const = 0;
    virtual bool validarReserva(int convidados, const std::string& inicio,
                                const std::string& fim) const = 0;

protected:
    bool validarPeriodo(int convidados, const std::string& inicio,
                        const std::string& fim, int duracaoMaximaMinutos) const;

private:
    int id_;
    std::string nome_;
    int capacidade_;
    double taxaBase_;
    std::string horaAbertura_;
    std::string horaFechamento_;
};
