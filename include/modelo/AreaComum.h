#pragma once

#include <string>

// classe base abstrata das areas comuns (salao, piscina e churrasqueira)
// nao da pra criar objeto dela direto, cada subclasse define sua taxa e suas regras de reserva
class AreaComum {
public:
    // recusa dados invalidos na criacao (capacidade maior que zero, taxa nao negativa, abertura antes do fechamento)
    AreaComum(int id, std::string nome, int capacidade, double taxaBase,
              std::string horaAbertura, std::string horaFechamento);
    // destrutor virtual pra apagar certo pelo ponteiro da classe base
    virtual ~AreaComum() = default;

    // getters
    int getId() const noexcept { return id_; }
    const std::string& getNome() const noexcept { return nome_; }
    int getCapacidade() const noexcept { return capacidade_; }
    double getTaxaBase() const noexcept { return taxaBase_; }
    const std::string& getHoraAbertura() const noexcept { return horaAbertura_; }
    const std::string& getHoraFechamento() const noexcept { return horaFechamento_; }

    // metodos virtuais puros: cada subclasse e obrigada a escrever os seus (polimorfismo)
    // texto do tipo, o mesmo que fica salvo no banco
    virtual std::string getTipo() const = 0;
    // calcula o valor da reserva (cada area cobra de um jeito)
    virtual double calcularTaxa(int convidados) const = 0;
    // diz se a reserva respeita as regras da area (capacidade, horario e duracao)
    virtual bool validarReserva(int convidados, const std::string& inicio,
                                const std::string& fim) const = 0;

protected:
    // parte comum das regras: convidados cabem, horario dentro do funcionamento e duracao maxima em minutos
    // as subclasses reaproveitam esse metodo
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
