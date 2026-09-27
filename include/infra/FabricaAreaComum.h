#pragma once

#include "infra/FabricaPorTipo.h"

class AreaComum;

// Cria a subclasse correspondente a area_comum.tipo no SQLite.
class FabricaAreaComum final : public FabricaPorTipo<AreaComum> {
public:
    // Associa o construtor de SalaoFestas ao valor salvo no banco.
    void registrarSalaoFestas(Criador criador);

    // Associa o construtor de Piscina ao valor salvo no banco.
    void registrarPiscina(Criador criador);

    // Associa o construtor de Churrasqueira ao valor salvo no banco.
    void registrarChurrasqueira(Criador criador);
};
