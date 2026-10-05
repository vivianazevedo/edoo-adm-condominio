#include "modelo/Piscina.h"

#include "infra/ErroCondominio.h"

// o texto tem que ser igual ao tipo salvo no banco (a fabrica usa ele)
std::string Piscina::getTipo() const { return "Piscina"; }

// piscina e gratis, mas recusa mais de 4 convidados
double Piscina::calcularTaxa(int convidados) const {
    if (convidados < 0 || convidados > getCapacidade() || convidados > 4) {
        throw ErroValidacao("Numero de convidados invalido para piscina");
    }
    return 0.0;
}

// no maximo 4 convidados e 2 horas de duracao
bool Piscina::validarReserva(int convidados, const std::string& inicio,
                            const std::string& fim) const {
    return convidados <= 4 && validarPeriodo(convidados, inicio, fim, 2 * 60);
}
