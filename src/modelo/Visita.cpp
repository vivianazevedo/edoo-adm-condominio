#include "modelo/Visita.h"
#include "infra/ErroCondominio.h"
#include <cctype>

using namespace std;

namespace {

// confere o formato AAAA-MM-DD HH:MM (o mesmo que fica no banco)
// com formato fixo, comparar texto e o mesmo que comparar horario
void validarDataHora(const string& valor, const string& campo) {
    bool formatoOk = valor.size() == 16 && valor[4] == '-' && valor[7] == '-' &&
                     valor[10] == ' ' && valor[13] == ':';
    for (size_t i = 0; formatoOk && i < valor.size(); ++i) {
        if (i == 4 || i == 7 || i == 10 || i == 13) continue;   // separadores
        if (!isdigit(static_cast<unsigned char>(valor[i]))) formatoOk = false;
    }
    if (!formatoOk) {
        throw ErroValidacao(campo + " invalida (use AAAA-MM-DD HH:MM): " + valor);
    }

    int mes = stoi(valor.substr(5, 2));
    int dia = stoi(valor.substr(8, 2));
    int hora = stoi(valor.substr(11, 2));
    int minuto = stoi(valor.substr(14, 2));
    if (mes < 1 || mes > 12 || dia < 1 || dia > 31 || hora > 23 || minuto > 59) {
        throw ErroValidacao(campo + " com valores fora do intervalo: " + valor);
    }
}

}  // namespace

// usa os setters pra validacao valer ja na criacao
// a entrada vem antes da saida porque a saida e comparada com ela
Visita::Visita(int visitanteId, int apartamentoId, int registradoPor,
               const string& entrada, const string& saida, int id)
    : id_(id), visitanteId_(0), apartamentoId_(0), registradoPor_(0) {
    setVisitanteId(visitanteId);
    setApartamentoId(apartamentoId);
    setRegistradoPor(registradoPor);
    setEntrada(entrada);
    setSaida(saida);
}

int Visita::getId() const { return id_; }
int Visita::getVisitanteId() const { return visitanteId_; }
int Visita::getApartamentoId() const { return apartamentoId_; }
int Visita::getRegistradoPor() const { return registradoPor_; }
const string& Visita::getEntrada() const { return entrada_; }
const string& Visita::getSaida() const { return saida_; }

void Visita::setId(int id) { id_ = id; }

void Visita::setVisitanteId(int visitanteId) {
    if (visitanteId <= 0) {
        throw ErroValidacao("A visita precisa de um visitante.");
    }
    visitanteId_ = visitanteId;
}

void Visita::setApartamentoId(int apartamentoId) {
    if (apartamentoId <= 0) {
        throw ErroValidacao("A visita precisa de um apartamento.");
    }
    apartamentoId_ = apartamentoId;
}

void Visita::setRegistradoPor(int funcionarioId) {
    if (funcionarioId <= 0) {
        throw ErroValidacao("A visita precisa de um funcionario que registrou.");
    }
    registradoPor_ = funcionarioId;
}

void Visita::setEntrada(const string& entrada) {
    validarDataHora(entrada, "Entrada");
    if (!saida_.empty() && saida_ < entrada) {
        throw ErroValidacao("A entrada nao pode ser depois da saida.");
    }
    entrada_ = entrada;
}

void Visita::setSaida(const string& saida) {
    if (!saida.empty()) {
        validarDataHora(saida, "Saida");
        if (saida < entrada_) {
            throw ErroValidacao("A saida nao pode ser antes da entrada.");
        }
    }
    saida_ = saida;
}

bool Visita::estaAberta() const { return saida_.empty(); }

string Visita::descricao() const {
    string texto = "Visitante " + to_string(visitanteId_) + " no apto " +
                   to_string(apartamentoId_) + ", entrada " + entrada_;
    texto += estaAberta() ? ", em aberto" : ", saida " + saida_;
    return texto;
}