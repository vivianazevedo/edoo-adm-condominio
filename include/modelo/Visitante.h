#pragma once
#include <string>
#include "modelo/Pessoa.h"



class Visitante : public Pessoa {
public:
    Visitante(std::string nome, std::string cpf, std::string telefone, int id = 0); // construtor 

    std::string tipo() const override; // devolve "visitante" (coluna pessoa.tipo do banco)
};