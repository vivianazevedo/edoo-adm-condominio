#pragma once
#include <string>
#include "modelo/Pessoa.h"



// visitante do condominio, herda de Pessoa e nao tem tabela propria no banco
class Visitante : public Pessoa {
public:
    Visitante(std::string nome, std::string cpf, std::string telefone, int id = 0); // construtor 

    std::string tipo() const override; // devolve "visitante" (coluna pessoa.tipo do banco)
};