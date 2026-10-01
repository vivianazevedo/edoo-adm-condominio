#include "modelo/AreaComum.h"

#include "infra/ErroCondominio.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <utility>

namespace {

int minutos(const std::string& hora) {
    if (hora.size() != 5 || hora[2] != ':' ||
        !std::isdigit(static_cast<unsigned char>(hora[0])) ||
        !std::isdigit(static_cast<unsigned char>(hora[1])) ||
        !std::isdigit(static_cast<unsigned char>(hora[3])) ||
        !std::isdigit(static_cast<unsigned char>(hora[4]))) {
        throw ErroValidacao("Horario deve estar em HH:MM");
    }
    const int h = std::stoi(hora.substr(0, 2));
    const int m = std::stoi(hora.substr(3, 2));
    if (h > 23 || m > 59) throw ErroValidacao("Horario fora do intervalo");
    return h * 60 + m;
}

bool somenteEspacos(const std::string& valor) {
    return std::all_of(valor.begin(), valor.end(), [](unsigned char c) {
        return std::isspace(c) != 0;
    });
}

}  // namespace

AreaComum::AreaComum(int id, std::string nome, int capacidade, double taxaBase,
                     std::string horaAbertura, std::string horaFechamento)
    : id_(id), nome_(std::move(nome)), capacidade_(capacidade), taxaBase_(taxaBase),
      horaAbertura_(std::move(horaAbertura)), horaFechamento_(std::move(horaFechamento)) {
    if (id < 0 || nome_.empty() || somenteEspacos(nome_) || capacidade <= 0 ||
        !std::isfinite(taxaBase) || taxaBase < 0 ||
        minutos(horaFechamento_) <= minutos(horaAbertura_)) {
        throw ErroValidacao("Dados invalidos da area comum");
    }
}

bool AreaComum::validarPeriodo(int convidados, const std::string& inicio,
                              const std::string& fim, int duracaoMaximaMinutos) const {
    const int inicioMinutos = minutos(inicio);
    const int fimMinutos = minutos(fim);
    return convidados >= 0 && convidados <= capacidade_ &&
           inicioMinutos >= minutos(horaAbertura_) &&
           fimMinutos <= minutos(horaFechamento_) &&
           fimMinutos > inicioMinutos &&
           fimMinutos - inicioMinutos <= duracaoMaximaMinutos;
}
