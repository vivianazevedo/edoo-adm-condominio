#include "modelo/SalaoFestas.h"

#include "infra/ErroCondominio.h"

std::string SalaoFestas::getTipo() const { return "SalaoFestas"; }

double SalaoFestas::calcularTaxa(int convidados) const {
    if (convidados < 0 || convidados > getCapacidade()) {
        throw ErroValidacao("Numero de convidados invalido");
    }
    return getTaxaBase() + 3.0 * convidados;
}

bool SalaoFestas::validarReserva(int convidados, const std::string& inicio,
                                const std::string& fim) const {
    return validarPeriodo(convidados, inicio, fim, 8 * 60);
}
