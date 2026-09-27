#pragma once

#include "infra/FabricaPorTipo.h"

class Pessoa;

// Cria Morador, Visitante ou Funcionario conforme pessoa.tipo no SQLite.
class FabricaPessoa final : public FabricaPorTipo<Pessoa> {
public:
    // Associa o construtor de Morador ao valor "morador" do banco.
    void registrarMorador(Criador criador);

    // Associa o construtor de Visitante ao valor "visitante" do banco.
    void registrarVisitante(Criador criador);

    // Associa o construtor de Funcionario ao valor "funcionario" do banco.
    void registrarFuncionario(Criador criador);
};
