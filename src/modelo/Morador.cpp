#include "modelo/Morador.h"
#include "infra/ErroCondominio.h"

using namespace std; 



Morador::Morador(string nome, string cpf, string telefone, int apartamentoId,
                 TipoOcupacao tipoOcupacao, string dataEntrada, int id)
    : Pessoa(nome, cpf, telefone, id), tipoOcupacao_(tipoOcupacao) {
    setApartamentoId(apartamentoId);
    setDataEntrada(dataEntrada);
} // construtor 

string Morador::tipo() const {
    return "morador";
}

void Morador::setApartamentoId(int apartamentoId) {
    if (apartamentoId <= 0) {
        throw ErroValidacao("O morador precisa estar vinculado a um apartamento.");
    }
    apartamentoId_ = apartamentoId;
}

void Morador::setDataEntrada(const string& dataEntrada) {
    dataEntrada_ = dataIsoValida(dataEntrada, "Data de entrada");
}