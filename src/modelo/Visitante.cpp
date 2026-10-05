#include "modelo/Visitante.h"

using namespace std;

// so repassa os dados pra Pessoa
Visitante::Visitante(string nome, string cpf, string telefone, int id) : Pessoa(nome, cpf, telefone, id) {}

// texto que e salvo na coluna tipo da tabela pessoa
string Visitante::tipo() const {
    return "visitante";
} 