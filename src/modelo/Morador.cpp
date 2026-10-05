#include "modelo/Morador.h"
#include "infra/ErroCondominio.h"

using namespace std; 



// os dados comuns vao pra Pessoa e o resto passa pelos setters que validam
Morador::Morador(string nome, string cpf, string telefone, int apartamentoId,
                 TipoOcupacao tipoOcupacao, string dataEntrada, int id)
    : Pessoa(nome, cpf, telefone, id), tipoOcupacao_(tipoOcupacao) {
    setApartamentoId(apartamentoId);
    setDataEntrada(dataEntrada);
} // construtor 

// texto que e salvo na coluna tipo da tabela pessoa
string Morador::tipo() const {
    return "morador";
}

// todo morador precisa de apartamento (id maior que zero)
void Morador::setApartamentoId(int apartamentoId) {
    if (apartamentoId <= 0) {
        throw ErroValidacao("O morador precisa estar vinculado a um apartamento.");
    }
    apartamentoId_ = apartamentoId;
}

// confere o formato AAAA-MM-DD da data de entrada
void Morador::setDataEntrada(const string& dataEntrada) {
    dataEntrada_ = dataIsoValida(dataEntrada, "Data de entrada");
}