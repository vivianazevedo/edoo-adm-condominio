#include "modelo/Visitante.h"

using namespace std;

Visitante::Visitante(string nome, string cpf, string telefone, int id) : Pessoa(nome, cpf, telefone, id) {}

string Visitante::tipo() const {
    return "visitante";
} 