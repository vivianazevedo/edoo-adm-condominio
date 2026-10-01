#include "modelo/Churrasqueira.h"

#include "infra/ErroCondominio.h"

std::string Churrasqueira::getTipo() const { return "Churrasqueira"; }

double Churrasqueira::calcularTaxa(int convidados) const {
    if (convidados < 0 || convidados > getCapacidade()) {
        throw ErroValidacao("Numero de convidados invalido");
    }
    return 40.0;
}

bool Churrasqueira::validarReserva(int convidados, const std::string& inicio,
                                  const std::string& fim) const {
    return validarPeriodo(convidados, inicio, fim, 4 * 60);
}
