#pragma once 
#include <string>
#include "modelo/Pessoa.h"



enum class TipoOcupacao { Proprietario, Inquilino, Dependente}; //forma de ocupacao

class Morador : public Pessoa {

    private:
        int apartamentoId_ = 0;
        TipoOcupacao tipoOcupacao_ = TipoOcupacao::Proprietario;
        std::string dataEntrada_;

    public: 
        Morador(std::string nome, std::string cpf, std::string telefone, int apartamentoId, // construtor 
        TipoOcupacao tipoOcupacao, std::string dataEntrada, int id = 0);


        std::string tipo() const override;

        int apartamentoId() const { return apartamentoId_; }
        TipoOcupacao tipoOcupacao() const { return tipoOcupacao_; }
        const std::string& dataEntrada() const { return dataEntrada_; }

        void setApartamentoId(int apartamentoId);
        void setTipoOcupacao(TipoOcupacao tipoOcupacao) { tipoOcupacao_ = tipoOcupacao; }
        void setDataEntrada(const std::string& dataEntrada);
};