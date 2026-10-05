#pragma once

#include "infra/FabricaPorTipo.h"

class Pessoa;

// cria Morador, Visitante ou Funcionario conforme o tipo salvo na tabela pessoa
class FabricaPessoa final : public FabricaPorTipo<Pessoa> {
public:
    // liga o texto morador a funcao que cria um Morador
    void registrarMorador(Criador criador);

    // liga o texto visitante a funcao que cria um Visitante
    void registrarVisitante(Criador criador);

    // liga o texto funcionario a funcao que cria um Funcionario
    void registrarFuncionario(Criador criador);
};
