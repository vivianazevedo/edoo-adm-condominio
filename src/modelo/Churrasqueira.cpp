#include "modelo/Churrasqueira.h"

#include "infra/ErroCondominio.h"

// o texto tem que ser igual ao tipo salvo no banco (a fabrica usa ele)
std::string Churrasqueira::getTipo() const { return "Churrasqueira"; }

// taxa fixa de 40 reais, nao importa o numero de convidados (so recusa numero invalido)
double Churrasqueira::calcularTaxa(int convidados) const {
    if (convidados < 0 || convidados > getCapacidade()) {
        throw ErroValidacao("Numero de convidados invalido");
    }
    return 40.0;
}

// reaproveita o validarPeriodo da base com duracao maxima de 4 horas
bool Churrasqueira::validarReserva(int convidados, const std::string& inicio,
                                  const std::string& fim) const {
    return validarPeriodo(convidados, inicio, fim, 4 * 60);
}
