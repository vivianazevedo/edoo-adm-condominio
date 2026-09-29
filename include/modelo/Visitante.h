#pragma once
#include <string>
#include "modelo/Pessoa.h"

using namespace std;


class Visitante : public Pessoa {
public:
    Visitante(string nome, string cpf, string telefone, int id = 0); // construtor 

    string tipo() const override; // devolve "visitante" (coluna pessoa.tipo do banco)
};