// teste dos modelos: taxa e regras de cada area comum, e as validacoes da Reserva (data e periodo)
#include "infra/ErroCondominio.h"
#include "modelo/Churrasqueira.h"
#include "modelo/Piscina.h"
#include "modelo/Reserva.h"
#include "modelo/SalaoFestas.h"

#include <stdexcept>

namespace {

// joga excecao se a condicao for falsa, e assim que o teste falha
void exigir(bool condicao, const char* mensagem) {
    if (!condicao) throw std::runtime_error(mensagem);
}

}  // namespace

int main() {
    SalaoFestas salao(0, "Salao", 80, 250.0, "08:00", "23:00");
    Piscina piscina(0, "Piscina", 30, 0.0, "08:00", "22:00");
    Churrasqueira churrasqueira(0, "Churrasqueira", 20, 0.0, "10:00", "22:00");

    exigir(salao.getTipo() == "SalaoFestas" &&
               salao.validarReserva(50, "10:00", "18:00") &&
               !salao.validarReserva(50, "10:00", "18:01") &&
               !salao.validarReserva(81, "10:00", "12:00") &&
               salao.calcularTaxa(50) == 400.0,
           "Regras do salao incorretas");
    exigir(piscina.getTipo() == "Piscina" &&
               piscina.validarReserva(4, "08:00", "10:00") &&
               !piscina.validarReserva(5, "08:00", "10:00") &&
               !piscina.validarReserva(4, "08:00", "10:01") &&
               piscina.calcularTaxa(4) == 0.0,
           "Regras da piscina incorretas");
    exigir(churrasqueira.getTipo() == "Churrasqueira" &&
               churrasqueira.validarReserva(20, "10:00", "14:00") &&
               !churrasqueira.validarReserva(20, "09:59", "13:59") &&
               !churrasqueira.validarReserva(20, "10:00", "14:01") &&
               churrasqueira.calcularTaxa(20) == 40.0,
           "Regras da churrasqueira incorretas");

    Reserva reserva(0, 1, 1, "2028-02-29", "10:00", "11:00", 4,
                    StatusReserva::ATIVA, 0.0);
    exigir(reserva.getStatus() == StatusReserva::ATIVA &&
               reserva.getData() == "2028-02-29" && reserva.getNumConvidados() == 4,
           "Dados da reserva incorretos");
    reserva.setStatus(StatusReserva::CANCELADA);
    exigir(reserva.getStatus() == StatusReserva::CANCELADA,
           "Cancelamento da reserva nao alterou o estado");

    bool dataInvalidaRejeitada = false;
    try {
        Reserva invalida(0, 1, 1, "2027-02-29", "10:00", "11:00", 1,
                         StatusReserva::ATIVA, 0.0);
    } catch (const ErroValidacao&) {
        dataInvalidaRejeitada = true;
    }
    exigir(dataInvalidaRejeitada, "Data inexistente aceita na reserva");

    bool periodoInvalidoRejeitado = false;
    try {
        Reserva invalida(0, 1, 1, "2028-02-29", "11:00", "10:00", 1,
                         StatusReserva::ATIVA, 0.0);
    } catch (const ErroValidacao&) {
        periodoInvalidoRejeitado = true;
    }
    exigir(periodoInvalidoRejeitado, "Periodo invertido aceito na reserva");
}
