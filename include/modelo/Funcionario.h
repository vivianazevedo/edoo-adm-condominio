#pragma once
#include <string>
#include "modelo/Pessoa.h"



enum class Cargo { Porteiro, Zelador, Faxineiro, Administrador }; // cargos possiveis


// funcionario do condominio, herda de Pessoa (o cargo e um atributo, so o Porteiro registra visitas)
class Funcionario : public Pessoa {


    private:
        Cargo cargo_ = Cargo::Porteiro;
        std::string turno_;
        std::string dataAdmissao_;
    public:
        Funcionario(std::string nome, std::string cpf, std::string telefone, Cargo cargo, // construtor 
                    std::string turno, std::string dataAdmissao, int id = 0);

        std::string tipo() const override; // devolve "funcionario" (coluna pessoa.tipo do banco).

        Cargo cargo() const { return cargo_; }
        const std::string& turno() const { return turno_; }
        const std::string& dataAdmissao() const { return dataAdmissao_; }

        bool ehPorteiro() const { return cargo_ == Cargo::Porteiro; }// true se o cargo for porteiro

        void setCargo(Cargo cargo) { cargo_ = cargo; }
        // turno e data passam pelas validacoes da Pessoa
        void setTurno(const std::string& turno);
        void setDataAdmissao(const std::string& dataAdmissao);

};