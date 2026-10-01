#pragma once
#include <string>
#include "modelo/Pessoa.h"

using namespace std;


enum class Cargo { Porteiro, Zelador, Faxineiro, Administrador }; //cargo do funcionario 


class Funcionario : public Pessoa {


    private:
        Cargo cargo_ = Cargo::Porteiro;
        string turno_;
        string dataAdmissao_;
    public:
        Funcionario(string nome, string cpf, string telefone, Cargo cargo, // construtor 
                    string turno, string dataAdmissao, int id = 0);

        string tipo() const override; // devolve "funcionario" (coluna pessoa.tipo do banco).

        Cargo cargo() const { return cargo_; }
        const string& turno() const { return turno_; }
        const string& dataAdmissao() const { return dataAdmissao_; }

        bool ehPorteiro() const { return cargo_ == Cargo::Porteiro; }// retorna true s for porteiro 

        void setCargo(Cargo cargo) { cargo_ = cargo; }
        void setTurno(const string& turno);
        void setDataAdmissao(const string& dataAdmissao);

};