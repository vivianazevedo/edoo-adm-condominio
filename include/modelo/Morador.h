#pragma once 
#include <string>
#include "modelo/Pessoa.h"

using namespace std; 


enum class TipoOcupacao { Proprietario, Inquilino, Dependente}; //forma de ocupacao

class Morador : public Pessoa {

    private:
        int apartamentoId_ = 0;
        TipoOcupacao tipoOcupacao_ = TipoOcupacao::Proprietario;
        string dataEntrada_;

    public: 
        Morador(string nome, string cpf, string telefone, int apartamentoId, // construtor 
        TipoOcupacao tipoOcupacao, string dataEntrada, int id = 0);


        string tipo() const override;

        int apartamentoId() const { return apartamentoId_; }
        TipoOcupacao tipoOcupacao() const { return tipoOcupacao_; }
        const string& dataEntrada() const { return dataEntrada_; }

        void setApartamentoId(int apartamentoId);
        void setTipoOcupacao(TipoOcupacao tipoOcupacao) { tipoOcupacao_ = tipoOcupacao; }
        void setDataEntrada(const string& dataEntrada);
};