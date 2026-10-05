#include "modelo/SalaoFestas.h"

#include "infra/ErroCondominio.h"

// o texto tem que ser igual ao tipo salvo no banco (a fabrica usa ele)
std::string SalaoFestas::getTipo() const { return "SalaoFestas"; }

// taxa base mais 3 reais por convidado
double SalaoFestas::calcularTaxa(int convidados) const {
    if (convidados < 0 || convidados > getCapacidade()) {
        throw ErroValidacao("Numero de convidados invalido");
    }
    return getTaxaBase() + 3.0 * convidados;
}

// reaproveita o validarPeriodo da base com duracao maxima de 8 horas
bool SalaoFestas::validarReserva(int convidados, const std::string& inicio,
                                const std::string& fim) const {
    return validarPeriodo(convidados, inicio, fim, 8 * 60);
}
