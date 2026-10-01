#include "modelo/Reserva.h"

#include "infra/ErroCondominio.h"

#include <cctype>
#include <cmath>
#include <utility>

namespace {

bool digito(char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; }

int numero(const std::string& texto, std::size_t inicio, std::size_t tamanho) {
    for (std::size_t i = inicio; i < inicio + tamanho; ++i) {
        if (!digito(texto[i])) throw ErroValidacao("Data ou horario invalido");
    }
    return std::stoi(texto.substr(inicio, tamanho));
}

void validarData(const std::string& data) {
    if (data.size() != 10 || data[4] != '-' || data[7] != '-') {
        throw ErroValidacao("Data deve estar em AAAA-MM-DD");
    }
    const int ano = numero(data, 0, 4);
    const int mes = numero(data, 5, 2);
    const int dia = numero(data, 8, 2);
    if (ano == 0 || mes < 1 || mes > 12) throw ErroValidacao("Data invalida");
    const int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    const bool bissexto = ano % 400 == 0 || (ano % 4 == 0 && ano % 100 != 0);
    const int limite = diasMes[mes - 1] + ((mes == 2 && bissexto) ? 1 : 0);
    if (dia < 1 || dia > limite) throw ErroValidacao("Data invalida");
}

int minutos(const std::string& hora) {
    if (hora.size() != 5 || hora[2] != ':') {
        throw ErroValidacao("Horario deve estar em HH:MM");
    }
    const int h = numero(hora, 0, 2);
    const int m = numero(hora, 3, 2);
    if (h > 23 || m > 59) throw ErroValidacao("Horario invalido");
    return h * 60 + m;
}

}  // namespace

Reserva::Reserva(int id, int moradorId, int areaId, std::string data,
                 std::string horaInicio, std::string horaFim, int numConvidados,
                 StatusReserva status, double valor)
    : id_(id), moradorId_(moradorId), areaId_(areaId), data_(std::move(data)),
      horaInicio_(std::move(horaInicio)), horaFim_(std::move(horaFim)),
      numConvidados_(numConvidados), status_(status), valor_(valor) {
    validarData(data_);
    if (id < 0 || moradorId <= 0 || areaId <= 0 || numConvidados < 0 ||
        !std::isfinite(valor) || valor < 0 ||
        (status != StatusReserva::ATIVA && status != StatusReserva::CANCELADA) ||
        minutos(horaFim_) <= minutos(horaInicio_)) {
        throw ErroValidacao("Dados invalidos da reserva");
    }
}
