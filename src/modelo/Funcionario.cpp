#include "modelo/Funcionario.h"

using namespace std;

// os dados comuns vao pra Pessoa, turno e data passam pelos setters que validam
Funcionario::Funcionario(string nome, string cpf, string telefone, Cargo cargo, // construtor 
                         string turno, string dataAdmissao, int id)
    : Pessoa(nome, cpf, telefone, id), cargo_(cargo) {
    setTurno(turno);
    setDataAdmissao(dataAdmissao);
}

string Funcionario::tipo() const { //retorna funcionario 
    return "funcionario";
}

void Funcionario::setTurno(const string& turno) { // turno 
    turno_ = textoObrigatorio(turno, "Turno");
}

// confere o formato AAAA-MM-DD da data de admissao
void Funcionario::setDataAdmissao(const string& dataAdmissao) { 
    dataAdmissao_ = dataIsoValida(dataAdmissao, "Data de admissao");
}