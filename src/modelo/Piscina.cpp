#include "modelo/Piscina.h"

#include "infra/ErroCondominio.h"

std::string Piscina::getTipo() const { return "Piscina"; }

double Piscina::calcularTaxa(int convidados) const {
    if (convidados < 0 || convidados > getCapacidade() || convidados > 4) {
        throw ErroValidacao("Numero de convidados invalido para piscina");
    }
    return 0.0;
}

bool Piscina::validarReserva(int convidados, const std::string& inicio,
                            const std::string& fim) const {
    return convidados <= 4 && validarPeriodo(convidados, inicio, fim, 2 * 60);
}
